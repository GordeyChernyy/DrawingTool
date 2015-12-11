//
//  Clip.cpp
//  DrawingTool
//
//  Created by Zerc on 12/10/15.
//
//

#include "timeline_types.h"

Clip::Clip(int w, int h) {
    setup(w,h);
}
void Clip::setup(int w, int h){
    frames.reserve(RESERVE_FRAMES);
    Frame *frame = new Frame;
    frame->setup(w, h);
    frames.push_back(*frame);
    currLayer = 0;     // count from 0
    numLayers = 1;    // count from 1
    currFrame = 0;    // count from 0
    inPoint = 0;
    outPoint = 0;
}
