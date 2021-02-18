#include "../include/FaceAnnotator.h"

#include <string>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>

using namespace std;

const string FaceAnnotator::FACE_LANDMARKS_PATH = "shape_predictor_68_face_landmarks.dat";

FaceAnnotator* FaceAnnotator::instance = nullptr;

FaceAnnotator::FaceAnnotator() {
    dlib::deserialize(FaceAnnotator::FACE_LANDMARKS_PATH) >> faceLandmarkAnnotator;
}

FaceAnnotator* FaceAnnotator::getFaceAnnotatorInstance() {
    if (!instance)
        instance = new FaceAnnotator();
    
    return instance;
}

dlib::full_object_detection FaceAnnotator::annotateFace(const cv::Mat& frame, const dlib::rectangle& object) {
    dlib::cv_image<dlib::bgr_pixel> image(frame);
    return faceLandmarkAnnotator(image, object);
}