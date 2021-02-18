#ifndef _DEPTH_INTERFACE_H
#define _DEPTH_INTERFACE_H

#include "Face.h"

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

class DepthInterface {
    public:
        virtual ~DepthInterface() {};
        virtual float getDepth(cv::Mat& frame, rs2::depth_frame& depthFrame, Face& face) = 0;
};

#endif