#include "../include/Sensor.h"

Sensor* Sensor::instance = NULL;

Sensor::Sensor(): pipe(), sensor(), configs() {
    this->configs.enable_stream(RS2_STREAM_DEPTH,848,480,RS2_FORMAT_Z16,90);
    this->configs.enable_stream(RS2_STREAM_COLOR);
};

float Sensor::getSensorDepthScale() {
    for(rs2::sensor& sensor : this->sensor.query_sensors())
        if(rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>())
            return dpt.get_depth_scale();

    throw std::runtime_error("Device does not have a depth sensor");
};

cv::Size Sensor::setCropSize(rs2::video_stream_profile& profile, const float whRatio) {
    if(profile.width() / (float)profile.height() > whRatio)
        return cv::Size(static_cast<int>(profile.height() * whRatio),profile.height());
    else
        return cv::Size(static_cast<int>(profile.width() * whRatio),profile.width());
};

Sensor* Sensor::getSensorInstance() {
    if (instance == NULL)
        instance = new Sensor();

    return instance;
}

cv::Rect Sensor::initPipeline() try {
        const size_t inWidth = 300;
        const size_t inHeight = 300;
        const float whRatio = inWidth / (float)inHeight;

        auto config = pipe.start();
        auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();

        this->sensor = config.get_device();

        cv::Size cropSize = this->setCropSize(profile,whRatio);

        return cv::Rect(cv::Point((profile.width() - cropSize.width) / 2,
                        (profile.height() - cropSize.height) / 2), cropSize);
} catch(const rs2::error & e){
    std::cerr<<"RealSense error calling "<< e.get_failed_function() << "(" << e.get_failed_args() << "):\n     "<< e.what() <<std::endl;
    exit(1);
};

rs2::pipeline& Sensor::getSensorPipeline() {
    return this->pipe;
}