#include "../include/FaceBox.h"
#include "../include/Coordinates.hpp"

#include<string>
#include <opencv2/opencv.hpp>

using namespace std;

FaceBox::FaceBox(): coordinates() {};

FaceBox::~FaceBox() {
    delete line1Point1;
    delete line1Point2;
    delete line2Point1;
    delete line2Point2;
    delete ellipseAxes;
    delete ellipseCenter;
}

void FaceBox::draw(cv::Mat& frame) {
    this->drawTopLeft(frame);
    this->drawTopRight(frame);
    this->drawBottomLeft(frame);
    this->drawBottomRight(frame);

    this->putText(frame);
};

void FaceBox::drawTopLeft(cv::Mat &frame) {
    this->setTopLeftPoints();

    cv::line(frame,*line1Point1,*line1Point2,boxColor,thickness);
    cv::line(frame,*line2Point1,*line2Point2,boxColor,thickness);
    cv::ellipse(frame,*ellipseCenter,*ellipseAxes,180,0,90,boxColor,thickness);
};

void FaceBox::drawTopRight(cv::Mat &frame) {
    this->setTopRightPoints();

    cv::line(frame, *line1Point1, *line1Point2, boxColor, thickness);
    cv::line(frame, *line2Point1, *line2Point2, boxColor, thickness);
    cv::ellipse(frame, *ellipseCenter, *ellipseAxes, 270, 0, 90, boxColor, thickness);
};

void FaceBox::drawBottomLeft(cv::Mat &frame) {
    this->setBottomLeftPoints();

    cv::line(frame, *line1Point1, *line1Point2, boxColor, thickness);
    cv::line(frame, *line2Point1, *line2Point2, boxColor, thickness);
    cv::ellipse(frame, *ellipseCenter, *ellipseAxes, 90, 0, 90, boxColor, thickness);
};

void FaceBox::drawBottomRight(cv::Mat &frame) {
    this->setBottomRightPoints();

    cv::line(frame, *line1Point1, *line1Point2, boxColor, thickness);
    cv::line(frame, *line2Point1, *line2Point2, boxColor, thickness);
    cv::ellipse(frame, *ellipseCenter, *ellipseAxes, 0, 0, 90, boxColor, thickness);
};

void FaceBox::putText(cv::Mat& frame) {
    cv::Point startPoint(coordinates.x1(), coordinates.y1() - 10);
    cv::putText(frame, text, startPoint, font, 0.5, boxColor, 1);
};

void FaceBox::setTopLeftPoints() {
    line1Point1 = new cv::Point(coordinates.x1()+radius,coordinates.y1());
    line1Point2 = new cv::Point(coordinates.x1()+radius+diameter,coordinates.y1());

    line2Point1 = new cv::Point(coordinates.x1(),coordinates.y1()+radius);
    line2Point2 = new cv::Point(coordinates.x1(), coordinates.y1()+radius+diameter);

    ellipseCenter = new cv::Point(coordinates.x1()+radius,coordinates.y1()+radius);
    ellipseAxes = new cv::Point(radius, radius);
};

void FaceBox::setTopRightPoints() {
    line1Point1 = new cv::Point(coordinates.x2()-radius,coordinates.y1());
    line1Point2 = new cv::Point(coordinates.x2()-radius-diameter,coordinates.y1());

    line2Point1 = new cv::Point(coordinates.x2(),coordinates.y1()+radius);
    line2Point2 = new cv::Point(coordinates.x2(),coordinates.y1()+radius+diameter);

    ellipseCenter = new cv::Point(coordinates.x2()-radius,coordinates.y1()+radius);
    ellipseAxes = new cv::Point(radius, radius);
};

void FaceBox::setBottomLeftPoints() {
    line1Point1 = new cv::Point(coordinates.x1() + radius, coordinates.y2());
    line1Point2 = new cv::Point(coordinates.x1() + radius + diameter, coordinates.y2());

    line2Point1 = new cv::Point(coordinates.x1(), coordinates.y2() - radius);
    line2Point2 = new cv::Point(coordinates.x1(), coordinates.y2() - radius - diameter);

    ellipseCenter = new cv::Point(coordinates.x1() + radius, coordinates.y2() - radius);
    ellipseAxes = new cv::Point(radius, radius);
};

void FaceBox::setBottomRightPoints() {
    line1Point1 = new cv::Point(coordinates.x2() - radius, coordinates.y2());
    line1Point2 = new cv::Point(coordinates.x2() - radius - diameter, coordinates.y2());

    line2Point1 = new cv::Point(coordinates.x2(), coordinates.y2() - radius);
    line2Point2 = new cv::Point(coordinates.x2(), coordinates.y2() - radius - diameter);

    ellipseCenter = new cv::Point(coordinates.x2() - radius, coordinates.y2() - radius);
    ellipseAxes = new cv::Point(radius, radius);
};

void FaceBox::setCoordinates(Coordinates coordinates_) {
    this->coordinates.set_x1(coordinates_.x1());
    this->coordinates.set_x2(coordinates_.x2());
    this->coordinates.set_y1(coordinates_.y1());
    this->coordinates.set_y2(coordinates_.y2());
};
void FaceBox::setColor(cv::Scalar color) {
    this->boxColor = color;
};
void FaceBox::setThickness(int thickness_) {
    this->thickness = thickness_;
};
void FaceBox::setFont(int font_) {
    this->font = font_;
};
void FaceBox::setText(string text_) {
    this->text = text_;
};