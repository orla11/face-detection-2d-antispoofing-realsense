#include "../include/Parser.h"
#include "../include/LivenessDetector.hpp"
#include "../include/FaceDetector.h"
#include "../include/Sensor.h"
#include "../include/Face.h"
#include "../include/InfoArea.h"
#include "../include/FaceBox.h"
#include "../include/infoType.h"

#ifndef _CURRENT_LIBS
#define _CURRENT_LIBS

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <ctime>

#include "../lib/cv-helpers.hpp"
#include "../lib/utils.hpp"
#include "../include/rs_frame_image.h"

#endif

int FRAMES = 0;

int main(int argc, char** argv) try {

    // Argument Parser Initialization
    Parser argumentParser;
    argumentParser.initDefaultOptions();
    auto result = argumentParser.parse(argc,argv);

    // Loading face detector from disk
    std::cout<<"[INFO] initializing..."<<std::endl;
    // ----------------------------------------------------------------------------------
    rs2::align align_to(RS2_STREAM_COLOR);

    cv::Mat frame;

    InfoArea infoArea;
    FaceBox faceBox;

    infoArea.setFontSize(0.5);
    infoArea.setFont(cv::FONT_HERSHEY_SIMPLEX);
    infoArea.setInfoType(alertType::info);

    Sensor* sensor = Sensor::getSensorInstance();
    cv::Rect crop = sensor->initPipeline();

    float confidence = result["confidence"].as<float>();
    FaceDetector faceDetector;
    faceDetector.setConfidence(confidence);

    LivenessDetector livenessDetector;
    livenessDetector.setBlinkConfigs(result["blink"].as<bool>(),result["blinks_number"].as<int>());
    // ---------------------------------------------------

    // Program core processing
    std::cout<<"[INFO] starting live view..."<<std::endl;
    // ---------------------------------------------------
    while(true){

        // Wait the next set or frames
        auto data = sensor->getSensorPipeline().wait_for_frames();

        // Spatially aligning frames
        data = align_to.process(data);

        // Get depth and color frame
        auto colorFrame = data.get_color_frame();
        auto depthFrame = data.get_depth_frame();

        // If only a new depth frame is received but
        // the color frame did not update, continue
        static int lastFrameNumber = 0;
        if(colorFrame.get_frame_number() == lastFrameNumber) continue;
        lastFrameNumber = colorFrame.get_frame_number();

        // Convert RealSense frame to OpenCV matrix
        auto colorMat = frame_to_mat(colorFrame);
        auto depthMat = depth_frame_to_meters(depthFrame);

        cv::Mat frame(colorMat);

        // --------------------------------------------------
        // Face Detection - Liveness Detection - Face Recognition
        // -------------------------------------------------------
        faceDetector.detectFaces(frame,depthFrame,crop);

        std::string infoText = getDateTimeString() + " | Area x - Storage Access: ";
        int liveFrameHeight = frame.size().height / 2 + 180;

        // Info alert area Position
        cv::Point areaPoint1(0, liveFrameHeight - 25);
        cv::Point areaPoint2(frame.size().width, liveFrameHeight);
        infoArea.setPoints(areaPoint1, areaPoint2);

        infoArea.setText(infoText + "please identify...");
        infoArea.draw(frame);

        if (faceDetector.facesDetected()){

            if(faceDetector.moreThanOneFaceDetected()){
                infoArea.setText(infoText + "More than one person detected, cannot authorize.");
                infoArea.draw(frame);
            }

            if(faceDetector.oneFaceDetected()){

                faceBox.setCoordinates(faceDetector.getFirstFace().getCoordinates());

                if(faceDetector.getFaceDistance(0) <= static_cast<int>(result["distance"].as<float>()*1000)){

                    // if face is spoofed mark as red and log
                    if(FRAMES >= 45){

                        // Red Box
                        faceBox.setColor(cv::Scalar(0,0,255));
                        faceBox.setText("Unauthorized");
                        faceBox.draw(frame);

                        // Info alert area
                        infoArea.setText(infoText + "logged attempt...");
                        infoArea.draw(frame);

                        // TODO: logging access attempt via thread
                    }else{

                        // Grey Box
                        faceBox.setColor(cv::Scalar(204,204,204));
                        faceBox.setText("Verifying");
                        faceBox.draw(frame);

                        // Info alert area
                        infoArea.setText(infoText + "verifying...");
                        infoArea.draw(frame);

                        //TODO: authenticate via api and if recognized get name and draw 
                        // green box with name label and
                        // authorization status through thread

                    }

                    // check if face is spoofed and increment frames
                    if(livenessDetector.detectLiveness(frame,depthFrame,faceDetector.getFirstFace())){
                        FRAMES = 0;
                    }else {
                        FRAMES++;
                    }
                        
                }
            }
        
        } else {
            // Info alert area
            infoArea.setText(infoText + "plase identify...");
            infoArea.draw(frame);
        }

        // TODO: add parser check for resized window
        cv::Rect myRoi(320,0,720,540);
        cv::Mat liveFrame = frame(myRoi);

        // Creating Live View Window
        cv::imshow("Live View",liveFrame);

        if(cv::waitKey(1) > 0){
            break;
        }
    }
    // ------------------------------------------------------------

    cv::destroyAllWindows();

    return EXIT_SUCCESS;


}catch(const std::exception & e){

    std::cerr<<e.what()<<std::endl;
    return EXIT_FAILURE;

}