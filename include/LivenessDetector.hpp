#ifndef _LIVENESS_DETECTOR_H
#define _LIVENESS_DETECTOR_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

#include "Face.h"
#include "FaceAnnotator.h"

#include "../include/Sensor.h"

#include "../lib/cv-helpers.hpp"
#include "../lib/validate_face.hpp"

typedef typename dlib::full_object_detection dlib_fod;

class LivenessDetector {
    private:
        bool blink = false;
        int blinksNumber = 0;

    public:
        LivenessDetector() {};
        
        void enableBlinkDetection(int blinksNumber) {
            this->blink = true;
            this->blinksNumber = blinksNumber;
        };

        void disableBlinkDetection() {
            this->blink = false;
            this->blinksNumber = 0;
        };

        void setBlinkConfigs(bool blink_, int blinksNumber_) {
            this->blink = blink_;
            this->blinksNumber = blinksNumber_;
        };

        bool detectLiveness(cv::Mat& frame, rs2::depth_frame& depthFrame, Face& nearestFace) {
            
            cv::Rect object(
                nearestFace.getCoordinates().x1(),
                nearestFace.getCoordinates().y1(),
                nearestFace.getCoordinates().x2() - nearestFace.getCoordinates().x1(),
                nearestFace.getCoordinates().y2() - nearestFace.getCoordinates().y1()
            );

            dlib_fod annotatedFace = FaceAnnotator::getFaceAnnotatorInstance()
                                        ->annotateFace(frame,openCVToDlib(object));

            float deviceDepthScale = Sensor::getSensorInstance()->getSensorDepthScale();

            bool res = validate_face(depthFrame,deviceDepthScale,annotatedFace,blink,blinksNumber);
            
            return res;
        };
};

#endif