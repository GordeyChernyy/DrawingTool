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
    BrushBase(){
        brushCount++;
    }
    static int getBrushCount(){
        return brushCount;
    }
    static int brushCount;
    virtual void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col) = 0;
    virtual string name() = 0;
    virtual void draw() = 0;
};

#endif /* BrushBase_h */
