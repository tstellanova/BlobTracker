//
// Created by Todd Stellanova on 20161027
//

#include <opencv2/opencv.hpp>

#include "BlockTracker.h"
#include "TrackerContext.h"


BlockTracker::BlockTracker():
    _impl{ new TrackerContext()}
{
}

bool BlockTracker::setup(const std::string& src, const std::string& outWindow)
{
    _impl->mDeviceName = src;
    _impl->mOutWindowName = outWindow;

    //ensure that we have a window thread
    cv::startWindowThread();
    cv::namedWindow(_impl->mOutWindowName,CV_WINDOW_NORMAL);
    cv::moveWindow(_impl->mOutWindowName,40,40);

    _impl->mVideoCap = new cv::VideoCapture();
    if (NULL != _impl->mVideoCap) {
        _impl->mVideoCap->open(src);
        if (_impl->mVideoCap->isOpened()) {
            return true;
        }
    }

    return false;
}

void BlockTracker::teardown()
{
    if (NULL != _impl) {
        if (NULL != _impl->mVideoCap) {

            delete _impl->mVideoCap;
            _impl->mVideoCap = NULL;
        }
        delete _impl;
        _impl = NULL;
    }

}

BlockTracker::~BlockTracker()
{
}

bool BlockTracker::findBlockCenter(PixelPosition& foundPosition)
{
    cv::Mat rawImg;
    cv::Mat img;
    //read a video frame
    bool success = _impl->mVideoCap->read(rawImg);
    if (success) {
        cv::imshow(_impl->mOutWindowName, rawImg);

        //blur the main image so sharp highlights and small shadows are diffused
        cv::blur(rawImg, img, cv::Size(9,9));

        cv::Mat thresholdImg;
        _impl->threshWithDefaults(img,thresholdImg);

        //find all the contours in the foreground object mask
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(thresholdImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        _impl->mDisplayImg = rawImg;
        if (contours.size() > 0) {
            //we found at least one contour after thresholding
            // for debug: draw the contour
//            cv::Scalar color(0, 200, 0);
//            cv::drawContours(_impl->mDisplayImg, contours, 0, color, 2, 8, hierarchy, 0);

            //get the convex hull of the first contour
            std::vector<cv::Point> hull;
            std::vector<cv::Point> cnt = contours[0];
            cv::convexHull(cnt,hull);
            cv::Moments moments = cv::moments(hull, false);

            if (moments.m00 > 0) {
                foundPosition.x = (int)(moments.m10 / moments.m00);
                foundPosition.y = (int)(moments.m01 / moments.m00);
                return true;
            }
        }
    }

    return false; //not found
}

void BlockTracker::drawBlockTrace(const PixelPosition& position)
{
    cv::Point center(position.x, position.y);
    cv::circle(_impl->mDisplayImg, center, 20, cv::Scalar(0,255,128), 3);
    cv::imshow(_impl->mOutWindowName, _impl->mDisplayImg );
}

void BlockTracker::setTrackingColorHSV(uint8_t hue, uint8_t saturation, uint8_t value)
{
    cv::Scalar hsv(hue, saturation, value);
    _impl->setTrackingColorHSV(hsv );
}

void BlockTracker::spinTracker()
{
    PixelPosition pos;
    if (findBlockCenter(pos)) {
        drawBlockTrace(pos);
        //give some cycles to highgui to update screen, read input etc
        cv::waitKey(100);
    }
    else {
        printf("Couldn't find blob\n");
    }
}



BlockTracker::TrackerContext::TrackerContext():
        mVideoCap(NULL)
{}


void BlockTracker::TrackerContext::thresholdImageHSV(cv::Mat& inFrame,
                                     cv::Scalar& minHSV,
                                     cv::Scalar& maxHSV,
                                     cv::Mat& outFrame)
{
    //convert BGR to HSV
    cv::Mat hsvFrame;
    cv::cvtColor(inFrame, hsvFrame, CV_BGR2HSV);
    cv::inRange(hsvFrame,minHSV,maxHSV,outFrame);
}

void BlockTracker::TrackerContext::threshWithDefaults(cv::Mat& inFrame, cv::Mat& outFrame)
{
    thresholdImageHSV(inFrame, mMinHSVThreshold, mMaxHSVThreshold, outFrame);
}


void BlockTracker::TrackerContext::setTrackingColorHSV(cv::Scalar& hsv)
{
    //uses  H: 0 - 180, S: 0 - 255, V: 0 - 255

    mMidHSV  = hsv;

    //calculate upper and lower threshold bounds
    int hue, saturation, value;
    hue = hsv(0) - 18;
    if (hue < 0) {
        hue += 180;//wrap
    }
    saturation = hsv(1) - 50;
    if (saturation < 0) {
        saturation = 0;// pin to minimum
    }
    value = hsv(2) - 50;
    if (value < 0) {
        value = 0;// pin to minimum
    }
    mMinHSVThreshold = cv::Scalar(hue,saturation,value);

    hue = hsv(0) + 10;
    if (hue > 180) {
        hue = 180;//pin
    }
    saturation = hsv(1) + 50;
    if (saturation > 255) {
        saturation = 255;
    }
    value = hsv(2) + 50;
    if (value > 255) {
        value = 255;
    }
    mMaxHSVThreshold = cv::Scalar(hue,saturation,value);

}


