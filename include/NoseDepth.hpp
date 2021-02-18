#ifndef _NOSE_DEPTH_H
#define _NOSE_DEPTH_H

#include "DepthInterface.h"
#include "FaceAnnotator.h"
#include "Face.h"
#include "Sensor.h"

#include "../lib/cv-helpers.hpp"
#include "../lib/validate_face.hpp"

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

class NoseDepth : public DepthInterface {
    public:
        NoseDepth() {};
        ~NoseDepth() {};

        float getDepth(cv::Mat& frame, rs2::depth_frame& depthFrame, Face& face) { 
            cv::Rect object(
                face.getCoordinates().x1(),
                face.getCoordinates().y1(),
                face.getSize().width(),
                face.getSize().height()
            );

            dlib::full_object_detection annotatedFace = FaceAnnotator::getFaceAnnotatorInstance()->annotateFace(frame,openCVToDlib(object));
            float deviceDepthScale = Sensor::getSensorInstance()->getSensorDepthScale();
            
            return getNoseDistance(depthFrame,deviceDepthScale,annotatedFace);
        }
};

#endif