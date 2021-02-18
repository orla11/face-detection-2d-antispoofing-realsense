#ifndef _SENSOR_H
#define _SENSOR_H

#include <librealsense2/rs.hpp>
#include <librealsense2/hpp/rs_internal.hpp>
#include <opencv2/opencv.hpp>

class Sensor {
    private:
        rs2::device sensor;
        rs2::config configs;
        rs2::pipeline pipe;
        static Sensor* instance;

        Sensor();

        Sensor(const Sensor&) = delete;
        Sensor(Sensor&&) = delete;
        void operator=(Sensor const&) = delete;

        cv::Size setCropSize(rs2::video_stream_profile& profile, const float whRatio);

    public:
        static Sensor* getSensorInstance();
        cv::Rect initPipeline();
        float getSensorDepthScale();
        rs2::pipeline& getSensorPipeline();
};

#endif