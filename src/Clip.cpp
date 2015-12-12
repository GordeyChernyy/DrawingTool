//
//  Clip.cpp
//  DrawingTool
//
//  Created by Zerc on 12/10/15.
//
//

#include "timeline_types.h"

Clip::Clip() {
    frames.reserve(RESERVE_FRAMES);
    currLayer = 0;     // count from 0
    numLayers = 1;    // count from 1
    currFrame = 0;    // count from 0
    inPoint = 0;
    outPoint = 0;
}

