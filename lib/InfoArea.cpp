#include "../include/InfoArea.h"
#include "../include/infoType.h"

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;

InfoArea::InfoArea() {};

void InfoArea::draw(cv::Mat& frame) {
    this->drawArea(frame);
    this->putText(frame);
};

void InfoArea::drawArea(cv::Mat& frame) {
    cv::rectangle(frame,cv::Rect(startPoint,endPoint),areaColor,-1);
};

void InfoArea::putText(cv::Mat& frame) {
    int x = endPoint.x / 2;
    int y = startPoint.y + ((endPoint.y - startPoint.y) / 2);

    auto textSize = cv::getTextSize(text,font,fontSize,1,0);

    auto xBoundaryText = x - (textSize.width / 2);
    auto yBoundaryText = y + (textSize.height / 2);

    auto textCenter = cv::Point(xBoundaryText,yBoundaryText);

    cv::putText(frame,text,textCenter,font,fontSize,cv::Scalar(255,255,255),1);
};

// TODO: polymorphism??
void InfoArea::setInfoType(alertType type) {
    switch (type) {
        case alertType::warning:
            this->areaColor = cv::Scalar(51,153,255);
            break;
        case alertType::error:
            this->areaColor = cv::Scalar(0,0,255);
            break;
        case alertType::info:
            this->areaColor = cv::Scalar(0,0,0);
            break;
        case alertType::success:
            this->areaColor = cv::Scalar(0,255,0);
        default:
            this->areaColor = cv::Scalar(255,128,0);
            break;
    }
};

void InfoArea::setPoints(cv::Point startPoint_, cv::Point endPoint_) {
    this->startPoint = std::move(startPoint_);
    this->endPoint = std::move(endPoint_);
};
void InfoArea::setFontSize(double fontSize_) {
    this->fontSize = fontSize_;
};
void InfoArea::setFont(int font_) {
    this->font = font_;
};
void InfoArea::setText(std::string text_) {
    this->text = text_;
};