//
//  FramesFbo.h
//  DrawingTool
//
//  Created by Zerc on 9/15/15.
//
//
#include "ofMain.h"
#pragma once

class FramesFbo{
public:
    FramesFbo(){}
    
    vector<ofFbo> frames;
    int currentIndex;

    ofParameter<int> imagesTotalP;
    ofParameter<int> imagesTotal;
    ofParameter<int> speed;
    ofParameter<bool> reload;
    ofParameterGroup parameters;
    
    void setup(){
        currentIndex = 0;
        parameters.setName("FramesFbo");
        parameters.add(imagesTotalP.set("imagesTotal", 12, 2, 100));
        parameters.add(speed.set("speed", 3, 1, 10));
        parameters.add(reload.set("reload", true));
        reload.addListener(this, &FramesFbo::reloadFrames);
        imagesTotal = imagesTotalP;
        addFrames();
    }
    void addFrames(){
        for (int i=0; i<imagesTotal; i++) {
            ofFbo f;
            f.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
            f.begin();
            ofClear(255, 0);
            f.end();
            frames.push_back(f);
        }
    }
    void begin(){
        frames[currentIndex].begin();
    }
    void end(){
        frames[currentIndex].end();
    }
    ofFbo getCurrentFrame(){
        return frames[currentIndex];
    }
    void update(){
        if ( ofGetFrameNum()%speed==0){
            currentIndex++;
            if (currentIndex==imagesTotal) {
                currentIndex = 0;
            }
        }
    }
    void draw(){
        frames[currentIndex].draw(0, 0);
    }
    void reloadFrames(bool &input){
        imagesTotal = imagesTotalP;
        resize();
    }
    void resize(){
        frames.clear();
        addFrames();
    };
};