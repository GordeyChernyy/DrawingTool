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
    void setup(int w, int h, ofParameterGroup *rootParameters);
    ofFbo *getCurrFbo();
    void draw();
    void draw(int size);
    void draw(int a, int b);
    void autoFrameHandle();
};

#endif /* CanvasManager_hpp */
