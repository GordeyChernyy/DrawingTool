//
//  Clip.hpp
//  DrawingTool
//
//  Created by Zerc on 3/15/16.
//
//

#ifndef Clip_hpp
#define Clip_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Timeline.h"
#include "Crop.hpp"
class Clip {
public:
    vector<ofFbo> fbos;
    Crop crop;
    int i = 0;
    void saveFbos(Timeline *timeline){
        int frameCounter = 0;
        for(auto frame : *timeline->getFrames()){
            ofImage imgOut;
            ofFbo fbo;
            fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
            fbo.begin(); ofClear(0, 0); fbo.end();
            for(auto layer : *frame.getLayers()){
                fbo.begin();
                ofSetColor(255, 255);
                layer.getFbo()->draw(0, 0);
                fbo.end();
            }
            // crop here
            crop.crop(&fbo);
            crop.croppedFbo.readToPixels(imgOut.getPixels());
            imgOut.save("test" + ofToString(frameCounter) + ".png");
            frameCounter++;
        }
        
    };
    
};
#endif /* Clip_hpp */
