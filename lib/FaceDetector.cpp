#include "../include/FaceDetector.h"
#include "../include/FaceAnnotator.h"
#include "../include/Face.h"
#include "../include/Coordinates.hpp"
#include "../include/Size.hpp"
#include "../include/FaceDepth.hpp"

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

using namespace std;

const string FaceDetector::protoPath = "deploy.prototxt";
const string FaceDetector::modelPath = "res10_300x300_ssd_iter_140000.caffemodel";

FaceDetector::FaceDetector() {
    this->net = cv::dnn::readNetFromCaffe(FaceDetector::protoPath,FaceDetector::modelPath);
    this->mirror = false;
    this->confidence = 0.5;
};

FaceDetector::FaceDetector(const string& protoPath_, const string& modelPath_) {
    this->net = cv::dnn::readNetFromCaffe(protoPath_,modelPath_);
    this->mirror = false;
    this->confidence = 0.5;
};

FaceDetector::~FaceDetector() {
    delete facesCoordinates;
   net.~Net();
};

Face& FaceDetector::getFirstFace() const {
    return this->facesCoordinates->at(0);
};

void FaceDetector::setConfidence(float confidence_) {
    this->confidence = confidence_;
};

int FaceDetector::getFacesNumber(const cv::Mat& detectionMat) {
    int numberOfFaces = 0;

    for(int i=0; i<detectionMat.rows; i++) {
        if(detectionMat.at<float>(i,2) > this->confidence)
            numberOfFaces++;
    }

    return numberOfFaces;
};

int FaceDetector::getFaceDistance(int index) {
    return static_cast<int>(this->facesCoordinates->at(index).faceDepth());
}

bool FaceDetector::facesDetected() {
    return this->facesCoordinates->size() > 0;
};

bool FaceDetector::moreThanOneFaceDetected() {
    return this->facesCoordinates->size() > 1;
};

bool FaceDetector::oneFaceDetected() {
    return this->facesCoordinates->size() == 1;
};

int FaceDetector::getFacesNumber() {
    return this->facesCoordinates->size();
};

vector<Face>* FaceDetector::getFacesCoordinates() const {
    return facesCoordinates;
};


//! :/ function too long
void FaceDetector::detectFaces(cv::Mat& frame, rs2::depth_frame& depthFrame, cv::Rect& crop) {
    
    if(this->mirror)
        cv::flip(frame,frame,1);

    int width = frame.size().width;
    int height = frame.size().height;

    cv::Mat inputBlob = cv::dnn::blobFromImage(frame,1.0,cv::Size(300,300));

    this->net.setInput(inputBlob);
    cv::Mat detections = this->net.forward();

    cv::Mat detectionMat(detections.size[2],detections.size[3],CV_32F,detections.ptr<float>());

    int numberOfFaces(this->getFacesNumber(detectionMat));

    facesCoordinates = new vector<Face>(numberOfFaces,Face());

    for(int i=0;i<detectionMat.rows;i++) {
        
        if(thereAreNoFaces(numberOfFaces)) continue;

        float detectionConfidence = detectionMat.at<float>(i,2);

        if(isDetectionConfidenceWithinThreshold(detectionConfidence)){

            int x1 = computeXcoordinate(detectionMat,frame,make_pair(i,3));
            int y1 = computeYcoordinate(detectionMat,frame,make_pair(i,4));
            int x2 = computeXcoordinate(detectionMat,frame,make_pair(i,5));
            int y2 = computeYcoordinate(detectionMat,frame,make_pair(i,6));

            Coordinates coords(x1,x2,y1,y2);
            this->setFacesCoordinates(coords,i);

            Size size(height,width);
            this->setFacesSize(size,i);

            this->setFacesDepth(frame,depthFrame,i);
        }
    }
};

bool FaceDetector::thereAreNoFaces(int numberOfFaces) {
    return numberOfFaces == 0;
};

bool FaceDetector::isDetectionConfidenceWithinThreshold(float detectionConfidence) {
    return detectionConfidence > this->confidence;
};

int FaceDetector::computeXcoordinate(cv::Mat& detectionMat, cv::Mat& frame, const pair<int,int>& indexes) {
    return static_cast<int>(detectionMat.at<float>(indexes.first, indexes.second) * frame.size().width);
};

int FaceDetector::computeYcoordinate(cv::Mat& detectionMat, cv::Mat& frame, const pair<int,int>& indexes) {
    return static_cast<int>(detectionMat.at<float>(indexes.first, indexes.second) * frame.size().height);
};

void FaceDetector::setFacesCoordinates(Coordinates coordinates,int index) {
    facesCoordinates->at(index).setCoordinates(coordinates);
};

void FaceDetector::setFacesSize(Size size,int index) {
    facesCoordinates->at(index).setSize(size);
};

void FaceDetector::setFacesDepth(cv::Mat& frame, rs2::depth_frame& depthFrame,int index) {
    FaceDepth fDepth;
    facesCoordinates->at(index).setFaceDepth(static_cast<int>(fDepth.getDepth(frame,depthFrame,facesCoordinates->at(index)) * 1000));
};