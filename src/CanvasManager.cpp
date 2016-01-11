//
//  CanvasManager.cpp
//  DrawingTool
//
//  Created by Zerc on 1/4/16.
//
//

#include "CanvasManager.hpp"
void CanvasManager::setup(int w, int h, ofParameterGroup *rootParameters){
    parameters.setName("CanvasManager");
    parameters.add(currCanvasMode.set("currentCanvas", 0, 0, 0));
    parameters.add(timeline.parameters);
    rootParameters->add(parameters);
    timeline.setup(w / 8, h * .75, w * (6.0/8.0), h * .2);
}
ofFbo *CanvasManager::getCurrFbo(){
    switch (currCanvasMode) {
        case 0:
            return timeline.getCurFbo();
            break;
        default:
            break;
    }
}
Timeline *CanvasManager::getTimeline(){
    return &timeline;
}
void CanvasManager::draw(){
    switch (currCanvasMode) {
        case 0:
            timeline.draw();
            break;
        default:
            break;
    }
}
void CanvasManager::autoFrameHandle(){
    switch (currCanvasMode) {
        case 0:
            timeline.autoFrameHandle();
            break;
        default:
            break;
    }
}
