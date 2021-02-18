#ifndef _FACE_DEPTH_H
#define _FACE_DEPTH_H

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

#include "DepthInterface.h"
#include "FaceAnnotator.h"
#include "Face.h"
#include "Sensor.h"

#include "../lib/cv-helpers.hpp"
#include "../lib/validate_face.hpp"

class FaceDepth : public DepthInterface {
    public:
        FaceDepth() {};
        ~FaceDepth() {};

        float getDepth(cv::Mat& frame, rs2::depth_frame& depthFrame, Face& face) { 
            cv::Rect object(
                face.getCoordinates().x1(),
                face.getCoordinates().y1(),
                face.getCoordinates().x2() - face.getCoordinates().x1(),
                face.getCoordinates().y2() - face.getCoordinates().y1()
            );

            dlib::full_object_detection annotatedFace = FaceAnnotator::getFaceAnnotatorInstance()->annotateFace(frame,openCVToDlib(object));

            float deviceDepthScale = Sensor::getSensorInstance()->getSensorDepthScale();

            return getFaceDistance(depthFrame,deviceDepthScale,annotatedFace);
        }
};

#endif