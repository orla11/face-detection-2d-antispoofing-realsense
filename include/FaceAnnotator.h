#ifndef _FACE_ANNOTATOR_H
#define _FACE_ANNOTATOR_H

#include <string>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

using namespace std;

class FaceAnnotator {
    private:
        static const string FACE_LANDMARKS_PATH;
        dlib::shape_predictor faceLandmarkAnnotator;
        static FaceAnnotator *instance;

        FaceAnnotator();

    public:
        void operator=(FaceAnnotator const&) = delete;
        static FaceAnnotator* getFaceAnnotatorInstance();
        dlib::full_object_detection annotateFace(const cv::Mat& frame, const dlib::rectangle& object);
};

#endif