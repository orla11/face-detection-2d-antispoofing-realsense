#ifndef _VALIDATE_FACE
#define _VALIDATE_FACE

#include <librealsense2/rs.hpp>   // Include RealSense Cross Platform API
#include <dlib/image_processing/full_object_detection.h>
#include <math.h>
#include <numeric>
#include <vector>
#include "../include/markup_68.h"

// eye blink aspect ration threshold
static const double EYE_AR_THRESH      = 0.3;
// # of consecutives frames the eye must be below the threshold
static const int EYE_AR_CONSEC_FRAMES  = 1;
// eye blink counter and total
static int COUNTER                     = 0;
static int TOTAL                       = 0;

/*
    Calculates the average depth for a range of two-dimentional points in face, such that:
        point(n) = face.part(n)
    and puts the result in *p_average_depth.

    Points for which no depth is available (is 0) are ignored and not factored into the average.

    Returns true if an average is available (at least one point has depth); false otherwise.
*/
static bool find_depth_from(
    rs2::depth_frame const & frame,
    float const depth_scale,
    dlib::full_object_detection const & face,
    markup_68 markup_from, markup_68 markup_to,
    float * p_average_depth
){
    uint16_t const * data = reinterpret_cast<uint16_t const *>(frame.get_data());

    float average_depth = 0;
    size_t n_points = 0;

    for(int i = markup_from; i <= markup_to; ++i){
        auto pt = face.part( i );
        auto depth_in_pixels = *(data + pt.y() * frame.get_width() + pt.x());

        if( !depth_in_pixels )
            continue;

        average_depth += depth_in_pixels * depth_scale;
        ++n_points;
    }
    
    if( !n_points )
        return false;
    if( p_average_depth )
        *p_average_depth = average_depth / n_points;
    return true;
}

/*
*   Computes the euclidean distance between to points
*   and return the result
*/
static double euclidean(dlib::point pt1, dlib::point pt2){

    double x = pt1.x() - pt2.x(); // computing number to square in next step
    double y = pt1.y() - pt2.y();
    double dist;

    dist = pow(x,2) + pow(y, 2); // computing euclidean distance
    dist = sqrt(dist);

    return dist;
}

/*
*   Computes the aspect ratio o the eye to determine if
*   eyelids are opened or closed
*/
static double eyeAspectRatio(std::vector<dlib::point> & eye){
    /** 
     *   BASED ON WORK BY Soukupovà and Cech IN THE 2016 PAPER
     *  "REAL-TIME EYE BLINK DETECTION USING FACIAL LANDMARKS"
     **/

    // computing distance between two sets of vertival eye landmarks (x,y)-coordinates
    double A = euclidean(eye.at(1),eye.at(5));
    double B = euclidean(eye.at(2),eye.at(4));

    // computing distance between the horizontal eye landmark (x,y)-coordinates
    double C = euclidean(eye.at(0),eye.at(3));

    // computing eye aspect ratio
    double ear = (A + B) / (2.0 * C);

    return ear;
}

/*
*   Detect eye blink: this function check eyeblink action
*   can be setted to detect a number of blinks in order to return true
*/
static bool detectBlink(
    dlib::full_object_detection const & face,
    int blinksNumber=0
){
    std::vector<dlib::point> leftEye;
    std::vector<dlib::point> rightEye;

    for(int i=markup_68::LEFT_EYE_FROM; i<= markup_68::LEFT_EYE_TO; i++)
        leftEye.push_back(face.part(i));

    for(int i=markup_68::RIGHT_EYE_FROM; i <= markup_68::RIGHT_EYE_TO; i++)
        rightEye.push_back(face.part(i));

    double leftEAR  = eyeAspectRatio(leftEye);
    double rightEAR = eyeAspectRatio(rightEye);

    // average the eye aspect ratio together for both eyes
    double ear = (leftEAR + rightEAR) / 2.0;

    // check if EAR is below blink threshold
    if(ear < EYE_AR_THRESH){
        COUNTER++;
    
    // otherwise the eye aspect ratio si not below the blink
    // threshold...
    }else{
        // if the eyes were closed for a sufficient number of frames
        // then increment the toal number of blinks
        if(COUNTER >= EYE_AR_CONSEC_FRAMES) TOTAL++;

        COUNTER = 0;
    }
    
    if(TOTAL < 1) return false;

    if(blinksNumber == 0){
        return true;
    }else{
        if(TOTAL >= blinksNumber){return true;}else{return false;} 
    }
    
}


/*
    Returns whether the given 68-point facial landmarks denote the face of a real
    person (and not a picture of one), using the depth data in depth_frame.

    See markup_68 for an explanation of the point topology.

    NOTE: requires the coordinates in face align with those of the depth frame.
*/
static bool validate_face(
    rs2::depth_frame const & frame,
    float const depth_scale,
    dlib::full_object_detection const & face,
    bool blink=false,
    int blinks_number=0
){
    // Collect all the depth information for the different facial parts

    // For the ears, only one may be visible -- we take the closer one!
    float left_ear_depth = 100, right_ear_depth = 100;
    if( !find_depth_from( frame, depth_scale, face, markup_68::RIGHT_EAR, markup_68::RIGHT_1, &right_ear_depth )
        && !find_depth_from( frame, depth_scale, face, markup_68::LEFT_1, markup_68::LEFT_EAR, &left_ear_depth ) )
        return false;
    float ear_depth = std::min( right_ear_depth, left_ear_depth );

    float chin_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::CHIN_FROM, markup_68::CHIN_TO, &chin_depth ) )
        return false;

    float nose_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::NOSE_RIDGE_2, markup_68::NOSE_TIP, &nose_depth ) )
        return false;

    float right_eye_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::RIGHT_EYE_FROM, markup_68::RIGHT_EYE_TO, &right_eye_depth ) )
        return false;
    float left_eye_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::LEFT_EYE_FROM, markup_68::LEFT_EYE_TO, &left_eye_depth ) )
        return false;
    float eye_depth = std::min( left_eye_depth, right_eye_depth );

    float mouth_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::MOUTH_OUTER_FROM, markup_68::MOUTH_INNER_TO, &mouth_depth ) )
        return false;

    // We just use simple heuristics to determine whether the depth information agrees with
    // what's expected: that the nose tip, for example, should be closer to the camera than
    // the eyes.

    // These heuristics are fairly basic but nonetheless serve to illustrate the point that
    // depth data can effectively be used to distinguish between a person and a picture of a
    // person...

    if( nose_depth >= eye_depth )                                           return false;
    if( eye_depth - nose_depth > 0.07f )                                    return false;

    if( nose_depth >= ear_depth)                                            return false;
    if( ear_depth - nose_depth > 0.18f )                                    return false;

    if( chin_depth > ear_depth)                                             return false;
    if( ear_depth - chin_depth > 0.15f )                                    return false;

    if( ear_depth <= eye_depth )                                            return false;
    if( mouth_depth <= nose_depth )                                         return false;
    if( mouth_depth > chin_depth )                                          return false;

    if(blink){
        if(!detectBlink(face,blinks_number)) return false;
    }

    // All the distances, collectively, should not span a range that makes no sense. I.e.,
    // if the face accounts for more than 20cm of depth, or less than 2cm, then something's
    // not kosher!
    // float x = std::max( { nose_depth, eye_depth, ear_depth, mouth_depth, chin_depth } );
    // float n = std::min( { nose_depth, eye_depth, ear_depth, mouth_depth, chin_depth } );
    // if( x - n > 0.20f )
    //     return false;
    // if( x - n < 0.02f )
    //     return false;

    return true;
}

/*
*   Compute the distance of the Nose from the camera
*/
static float getNoseDistance(
    rs2::depth_frame const & frame,
    float const depth_scale,
    dlib::full_object_detection const & face){

            float nose_depth;
            if(!find_depth_from( frame, depth_scale, face, markup_68::NOSE_RIDGE_2, markup_68::NOSE_TIP, &nose_depth)){
                return 0.0;
            }else{
                return nose_depth;
            }
}

/*
*   Compute the distance of the Face from the camera
*/
static float getFaceDistance(
    rs2::depth_frame const & frame,
    float const depth_scale,
    dlib::full_object_detection const & face){

    std::vector<float> face_depths;

    float left_ear_depth = 100, right_ear_depth = 100;
    if( !find_depth_from( frame, depth_scale, face, markup_68::RIGHT_EAR, markup_68::RIGHT_1, &right_ear_depth )
        && !find_depth_from( frame, depth_scale, face, markup_68::LEFT_1, markup_68::LEFT_EAR, &left_ear_depth ) )
        return 0.0;
    float ear_depth = std::min( right_ear_depth, left_ear_depth );

    face_depths.push_back(ear_depth);

    float chin_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::CHIN_FROM, markup_68::CHIN_TO, &chin_depth ) )
        return 0.0;

    face_depths.push_back(chin_depth);

    float nose_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::NOSE_RIDGE_2, markup_68::NOSE_TIP, &nose_depth ) )
        return 0.0;
    
    face_depths.push_back(nose_depth);

    float right_eye_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::RIGHT_EYE_FROM, markup_68::RIGHT_EYE_TO, &right_eye_depth ) )
        return 0.0;
    float left_eye_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::LEFT_EYE_FROM, markup_68::LEFT_EYE_TO, &left_eye_depth ) )
        return 0.0;
    float eye_depth = std::min( left_eye_depth, right_eye_depth );

    face_depths.push_back(eye_depth);

    float mouth_depth;
    if( !find_depth_from( frame, depth_scale, face, markup_68::MOUTH_OUTER_FROM, markup_68::MOUTH_INNER_TO, &mouth_depth ) )
        return 0.0;

    face_depths.push_back(mouth_depth);

    return (std::accumulate(std::begin(face_depths), std::end(face_depths), 0.0) / 5);
}

#endif