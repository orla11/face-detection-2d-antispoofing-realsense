#ifndef _INFO_AREA_H
#define _INFO_AREA_H

#include "DrawerInterface.h"
#include "infoType.h"

#include <opencv2/opencv.hpp>

class InfoArea: public DrawerInterface {
    private:
        cv::Point startPoint;
        cv::Point endPoint;
        cv::Scalar areaColor;
        double fontSize;
        int font;
        std::string text;

        void drawArea(cv::Mat& frame);
        void putText(cv::Mat& frame);

    public:
        InfoArea();
        void draw(cv::Mat& frame);

        void setPoints(cv::Point p1_, cv::Point p2_);
        void setInfoType(alertType type);
        void setFontSize(double fontSize_);
        void setFont(int font_);
        void setText(std::string text_);
};

#endif