//
// Created by Todd Stellanova on 20161027
//

#include <iostream>

#include "BlockTracker.h"
static const char* kWindowName = "Tracking";

int main() {

    BlockTracker bt;
    //TODO use relative path
    if (!bt.setup("/Users/todd/Documents/proj/blocktracker/data/red_blob_480.mov", kWindowName)) {
        printf("Couldn't setup!\n");
        return -1;
    }

    //set a tracking color that is roughly "dark red"
    bt.setTrackingColorHSV(170,208,70);

    for (int i = 0; i < 120; i++) {
        printf("frame: %d \n",i);
        bt.spinTracker();
    }

    bt.teardown();

    return 0;
}