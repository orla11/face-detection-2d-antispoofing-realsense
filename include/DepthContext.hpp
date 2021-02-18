#ifndef _DEPTH_CONTEXT_H
#define _DEPTH_CONTEXT_H

#include "DepthInterface.h"
#include "Face.h"

#include "../lib/cv-helpers.hpp"
#include "../lib/validate_face.hpp"

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

class DepthContext {
    private:
        DepthInterface* depth;

    public:
        DepthContext(DepthInterface* depth_ = nullptr) : depth(depth) {};
        ~DepthContext() {
            delete depth;
        }

        void setDepthContext(DepthInterface* depth_) {
            delete this->depth;
            this->depth = depth_;
        }

        float getDepth(cv::Mat& frame, rs2::depth_frame& depth_frame, Face* face) const {
            return this->depth->getDepth(frame,depth_frame,face);
        }
};

#endif