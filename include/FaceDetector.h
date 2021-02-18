#ifndef _DETECTOR_H
#define _DETECTOR_H

#include "../include/FaceAnnotator.h"
#include "../include/Face.h"
#include "../include/Coordinates.hpp"
#include "../include/Size.hpp"

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

using namespace std;

class FaceDetector {
    private:
        vector<Face>* facesCoordinates;

        static const string protoPath;
        static const string modelPath;

        cv::dnn::Net net;

        bool mirror;
        float confidence;

        bool thereAreNoFaces(int numberOfFaces);
        bool isDetectionConfidenceWithinThreshold(float detectionConfidence);
        int computeXcoordinate(cv::Mat& detectionMat, cv::Mat& frame, const pair<int,int>& indexes);
        int computeYcoordinate(cv::Mat& detectionMat, cv::Mat& frame, const pair<int,int>& indexes);

        void setFacesCoordinates(Coordinates coordinates,int index);
        void setFacesSize(Size size,int index);
        void setFacesDepth(cv::Mat& frame, rs2::depth_frame& depthFrame,int index);

        int getFacesNumber();

    public:
        FaceDetector();
        FaceDetector(const string& protoPath_, const string& modelPath_);
        ~FaceDetector();

        void setConfidence(float confidence_);

        void detectFaces(cv::Mat& frame, rs2::depth_frame& depthFrame, cv::Rect& crop);
        int getFacesNumber(const cv::Mat& detectionMat);
        vector<Face>* getFacesCoordinates() const;
        Face& getFirstFace() const;
        bool facesDetected();
        bool moreThanOneFaceDetected();
        bool oneFaceDetected();
        int getFaceDistance(int index);
};

#endif