//
//  CanvasManager.hpp
//  DrawingTool
//
//  Created by Zerc on 1/4/16.
//
//

#ifndef CanvasManager_hpp
#define CanvasManager_hpp

#include "ofMain.h"
#include "timeline_types.h"

class CanvasManager{
private:
    Timeline timeline;
    ofParameter<int> currCanvasMode;
public:
    Timeline *getTimeline();
    ofParameterGroup parameters;
    void setup(int w, int h);
    ofFbo *getCurrFbo();
    void draw();
    void autoFrameHandle();
};

#endif /* CanvasManager_hpp */
