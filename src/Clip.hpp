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
    ofxXmlSettings settings;
    Crop crop;
    ofDirectory dir;
    
    Clip(){
        currFrameNum = 0;
        speed = 4;
    }
    void load(string path){
        dir.listDir(path);
        
        // load fbos and settings
        for(int i = 0; i < dir.size(); i++){
            string name = dir.getFile(i).getBaseName();
            
            if(name == "settings"){ // set positons
                settings.loadFile(dir.getPath(i));
                int x = settings.getValue("framePosX" + ofToString(i), 0);
                int y = settings.getValue("framePosY" + ofToString(i), 0);
                pos.push_back(ofVec2f(x, y));
            }else{ // set fbos
                ofImage img;
                ofFbo fbo;
                
                img.load(dir.getPath(i));
                
                fbo.allocate(img.getWidth(), img.getHeight());
                fbo.begin();
                ofClear(0, 0);
                img.draw(0, 0);
                fbo.end();
                
                fbos.push_back(fbo);
            }
        }
    }
    void saveFbos(Timeline *timeline){
        
        clipName = ofSystemTextBoxDialog("Enter name for clip");
        
        vector<ofVec2f> posRelative;
        int frameCounter = 0;
        int boundsLeft = ofGetWidth();
        int boundsTop = ofGetHeight();
        int boundsRight = 0;
        int boundsBottom = 0;
        
        // walk through all frames
        for(auto frame : *timeline->getFrames()){
            ofImage imgOut;
            ofFbo fbo;
            fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
            fbo.begin(); ofClear(0, 0); fbo.end();
            int i = 0;
            for(auto layer : *frame.getLayers()){
                i++;
                fbo.begin();
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                ofSetColor(255, 255);
                layer.getFbo()->draw(0, 0);
                glDisable(GL_BLEND);
                fbo.end();
            }
            
            // crop, it's not crop currFbo, there is croppedFbo inside crop
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
            
            // save image
            imgOut.save("clips/"+clipName + "/" + ofToString(frameCounter) + ".png");
            frameCounter++;
        }
        int i = 0;
        for(auto p : posRelative){
            ofVec2f framePos = p-ofVec2f(boundsLeft, boundsTop);
            pos.push_back(framePos);
            settings.setValue("settings:framePosX" + ofToString(i), framePos.x);
            settings.setValue("settings:framePosY" + ofToString(i), framePos.y);
            i++;
        }
        // save settings
        settings.saveFile("clips/"+clipName + "/" + "settings.xml");
    }
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
