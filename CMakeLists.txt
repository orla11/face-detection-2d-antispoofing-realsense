#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(lidar_realsense)

# Save the command line compile commands in the build output
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Make project require C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_library(REALSENSE2_FOUND realsense2 HINTS ${LIBRARY_DIR} REQUIRED)
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

if(NOT REALSENSE2_FOUND)
    SET(REALSENSE2_FOUND "realsense2")
    message(WARN "Failed to find_library(realsense2)")
endif()

# add_executable(main ./src/main.cpp
#                 ./include/Parser.h ./include/markup_68.h ./include/rs_frame_image.h
#                 ./lib/cv-helpers.hpp ./lib/validate_face.hpp ./lib/cxxopts.hpp
#                 ./lib/InfoArea.cpp ./lib/utils.hpp ./lib/Parser.cpp)


add_executable(main ./src/main.cpp
                ./include/Parser.h ./include/markup_68.h ./include/rs_frame_image.h
                ./include/DepthInterface.h ./include/DrawerInterface.h ./include/Face.h
                ./include/FaceAnnotator.h ./include/FaceBox.h ./include/FaceDetector.h
                ./include/InfoArea.h ./include/infoType.h
                ./include/Coordinates.hpp ./include/DepthContext.hpp ./include/FaceDepth.hpp
                ./include/NoseDepth.hpp ./include/Sensor.h ./include/Size.hpp 
                ./include/LivenessDetector.hpp
                ./lib/cv-helpers.hpp ./lib/validate_face.hpp ./lib/cxxopts.hpp
                ./lib/Face.cpp ./lib/FaceAnnotator.cpp ./lib/FaceBox.cpp ./lib/FaceDetector.cpp
                ./lib/InfoArea.cpp ./lib/utils.hpp ./lib/Parser.cpp ./lib/Sensor.cpp)

configure_file(./models/res10_300x300_ssd_iter_140000.caffemodel 
                res10_300x300_ssd_iter_140000.caffemodel COPYONLY)

configure_file(./models/deploy.prototxt 
                deploy.prototxt COPYONLY)

configure_file(./models/shape_predictor_68_face_landmarks.dat 
                shape_predictor_68_face_landmarks.dat COPYONLY)

target_link_libraries(lidar_realsense ${REALSENSE2_FOUND})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(lidar_realsense dlib::dlib)
set_target_properties (lidar_realsense PROPERTIES FOLDER Examples)