cmake_minimum_required(VERSION 3.6)
project(blocktracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/BlockTracker.cpp inc/BlockTracker.h inc/TrackerContext.h)
include_directories(inc)

find_package(OpenCV REQUIRED)
#COMPONENTS core imgproc highgui)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(blocktracker ${SOURCE_FILES})
target_link_libraries(blocktracker ${OpenCV_LIBS})