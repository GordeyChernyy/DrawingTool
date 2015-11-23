//
//  BrushBase.h
//  DrawingTool
//
//  Created by adhulip on 11/22/15.
//
//

#ifndef BrushBase_h
#include "ofMain.h"
#pragma once
#define BrushBase_h
class BrushBase {
    public:
        virtual void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col) = 0;
};

#endif /* BrushBase_h */
