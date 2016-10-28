//
// Created by Todd Stellanova on 20161027
//

#ifndef BLOCKTRACKER_BLOCKTRACKER_H
#define BLOCKTRACKER_BLOCKTRACKER_H

#include <string>

struct PixelPosition {
    int x;
    int y;
};


/**
 * A class that demonstrates how to detect the largest blob
 * within a certain color range, using OpenCV's C++ API
 *
 *
 */
class BlockTracker {

public:
    BlockTracker();
    virtual ~BlockTracker();

    /**
     * You must call this method before eg spinTracker
     * @param src Video input file path
     * @param outWindow Name of window to display tracking
     * @return true if setup succeeded
     */
    virtual bool setup(const std::string& src, const std::string& outWindow);
    virtual void teardown();

    /**
     * Set a guideline tracking color using the HSV color space.
     * You should call this with an estimated color before trying to track the blob.
     *
     * @param hue Hue in 0 - 180 range
     * @param saturation Saturation in 0..255 range
     * @param value Value in 0..255 range
     */
    virtual void setTrackingColorHSV(uint8_t hue, uint8_t saturation, uint8_t value);

    /**
     * Call this method to try finding the blob once and display it in the output window.
     */
    virtual void spinTracker();

    /**
     *
     * @param foundPosition Center of the blob found, if any
     * @return true if the blob was found
     */
    virtual bool findBlockCenter(PixelPosition& foundPosition);

    /**
     * Display the blob center.
     * @param position The position at which to draw the blob trace
     */
    virtual void drawBlockTrace(const PixelPosition& position);


private:
    // forward declaration
    class TrackerContext;
    // private internal implementation (PIMPL)
    TrackerContext* _impl;

};


#endif //BLOCKTRACKER_BLOCKTRACKER_H
