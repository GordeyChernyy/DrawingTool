//
//  ClipManager.hpp
//  DrawingTool
//
//  Created by Zerc on 1/12/16.
//
//

#ifndef ClipManager_hpp
#define ClipManager_hpp

#include "ofMain.h"
#include "Timeline.h"

class ClipManager{
public:
    void saveClip(Timeline *curTimeline);
    void loadClip(Timeline *curTimeline, string clip, int frameNum, int layerNum);
    
private:
    //string clipDir;
    vector<Clip> clips;
};

class Clip {
public:
    void newFrame(ofFbo *fbo, int frameNum, int layerNum);
    vector<SavedFrame> frames;
private:
};

class SavedFrame {
public:
    ofFbo *fbo;
    int frameNum;
    int layerNum;
private:
};

void ClipManager::saveClip(Timeline *curTimeline) {
    Clip *newClip = new Clip;
    for(int i = 0; i < curTimeline->getNumFrames(); i++) {
        for(int j = 0; j < curTimeline->getNumLayers(); j++){
            if(curTimeline->isAllocated(i,j)) {
                newClip->newFrame(curTimeline->getCurFbo(), i, j);
            }
            curTimeline->setLayerNum( curTimeline->getCurLayerNum() + 1);
        }
        curTimeline->setFrameNum( curTimeline->getCurFrameNum() + 1);
    }
}

void Clip::newFrame(ofFbo *fbo, int frameNum, int layerNum) {
    SavedFrame *newFrame = new SavedFrame;
    newFrame->fbo = fbo;
    newFrame->frameNum = frameNum;
    newFrame->layerNum = layerNum;
    frames.push_back(newFrame);
}

#endif /* ClipManager_hpp */
