#ifndef _DRAWER_INTERFACE_H
#define _DRAWER_INTERFACE_H

#include <opencv2/opencv.hpp>

class DrawerInterface {
    public:
        virtual ~DrawerInterface() {};
        virtual void draw(cv::Mat& frame) = 0;
};

#endif