//
//  BrushBase.h
//  DrawingTool
//
//  Created by adhulip on 11/22/15.
//
//

#ifndef BrushBase_h
#include "ofMain.h"
#include "Timeline.h"
#pragma once
#define BrushBase_h
class BrushBase {
private:
    public:
    BrushBase(){
    }
    ofParameterGroup parameters;
    virtual void updateCanvas(Timeline *timeline, ofFbo *fbo, float x, float y, float pressure, ofColor col) = 0;
    virtual void onMouseDown(Timeline *timeline, ofFbo *fbo, int x, int y) = 0;
    virtual void onMouseUp(Timeline *timeline, ofFbo *fbo, int x, int y) = 0;
    virtual string name() = 0;
    virtual void draw() = 0;
};

#endif /* BrushBase_h */
