#ifndef _FACE_BOX_H
#define _FACE_BOX_H

#include "DrawerInterface.h"
#include "Coordinates.hpp"

#include<string>
#include <opencv2/opencv.hpp>

using namespace std;

class FaceBox : public DrawerInterface {
    private:
        Coordinates coordinates;
        cv::Scalar boxColor;
        int thickness = 2;
        int radius = 20;
        int diameter = 15;
        int font = cv::FONT_HERSHEY_COMPLEX;
        string text;

        cv::Point* line1Point1;
        cv::Point* line1Point2;

        cv::Point* line2Point1;
        cv::Point* line2Point2;

        cv::Point* ellipseCenter;
        cv::Point* ellipseAxes;

        void drawTopLeft(cv::Mat &frame);
        void drawTopRight(cv::Mat &frame);
        void drawBottomLeft(cv::Mat &frame);
        void drawBottomRight(cv::Mat &frame);

        void setTopLeftPoints();
        void setTopRightPoints();
        void setBottomLeftPoints();
        void setBottomRightPoints();

        void putText(cv::Mat& frame);

    public:
        FaceBox();
        ~FaceBox();
        void setCoordinates(Coordinates coordinates_);
        void setColor(cv::Scalar color);
        void setThickness(int thickness_);
        void setFont(int font_);
        void setText(string text_);
        void draw(cv::Mat &frame);
};

#endif