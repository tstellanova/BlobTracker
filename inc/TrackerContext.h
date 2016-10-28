//
// Created by Todd Stellanova on 20161027
//

#ifndef BLOCKTRACKER_TRACKERCONTEXT_H
#define BLOCKTRACKER_TRACKERCONTEXT_H


#include <string>
#include <opencv2/opencv.hpp>


/**
 * Internal implementation for BlockTracker
 */
class BlockTracker::TrackerContext {
public:
    std::string mOutWindowName;
    std::string mDeviceName;
    cv::VideoCapture *mVideoCap;
    cv::Scalar mMidHSV;
    cv::Scalar mMinHSVThreshold;
    cv::Scalar mMaxHSVThreshold;
    cv::Mat mDisplayImg;

public:
    TrackerContext();

    void setTrackingColorHSV(cv::Scalar &hsv);
    void threshWithDefaults(cv::Mat &inFrame, cv::Mat &outFrame);

    //Threshold the image by finding pixels that fall within the hsv range given
    void thresholdImageHSV(cv::Mat &inFrame,
                           cv::Scalar &minHSV,
                           cv::Scalar &maxHSV,
                           cv::Mat &outFrame);
};



#endif //BLOCKTRACKER_TRACKERCONTEXT_H
