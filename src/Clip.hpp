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
#include "ofxXmlSettings.h"
class Clip {
public:
    vector<ofFbo> fbos;
    vector<ofVec2f> pos;
    int speed;
    int currFrameNum;
    string clipName;
    
    Clip(){
        currFrameNum = 0;
        speed = 4;
    }
    ofxXmlSettings settings;
    Crop crop;
    int i = 0;
    
    void saveFbos(Timeline *timeline){
        clipName = ofSystemTextBoxDialog("Enter name for clip");
        vector<ofVec2f> posRelative;
        int frameCounter = 0;
        int boundsLeft = ofGetWidth();
        int boundsTop = ofGetHeight();
        int boundsRight = 0;
        int boundsBottom = 0;
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
            crop.crop(&fbo);
            // pos relative window size
            posRelative.push_back(ofVec2f(crop.left, crop.top));
            // set bounding box for all frames
            if(crop.left<boundsLeft){
                boundsLeft = crop.left;
            }
            if(crop.top<boundsTop){
                boundsTop = crop.top;
            }
            if(crop.right>boundsRight){
                boundsRight = crop.right;
            }
            if(crop.bottom>boundsBottom){
                boundsBottom = crop.bottom;
            }
            
            crop.croppedFbo.readToPixels(imgOut.getPixels());
            fbos.push_back(crop.croppedFbo);
            imgOut.save(clipName + "/" + ofToString(frameCounter) + ".png");
            frameCounter++;
        }
        for(auto p : posRelative){
            pos.push_back(p-ofVec2f(boundsLeft, boundsTop));
        }
        cout <<  "boundsLeft= " << boundsLeft  << endl;
        cout <<  "boundsRight= " << boundsRight  << endl;
        cout <<  "boundsTop= " << boundsTop  << endl;
        cout <<  "boundsBottom= " << boundsBottom  << endl;
    };
    void draw(){
        if(fbos.size()>0){
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            ofSetColor(255, 255);
            fbos[currFrameNum].draw(pos[currFrameNum]);
            glDisable(GL_BLEND);
            if(ofGetFrameNum()%speed==0){
                currFrameNum++;
                if(currFrameNum>fbos.size()-1){
                    currFrameNum = 0;
                }
            }
        }
    }
    
};
#endif /* Clip_hpp */
