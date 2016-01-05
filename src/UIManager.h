//
//  UIManager.h
//  DrawingTool
//
//  Created by Zerc on 11/25/15.
//
//
#pragma once
#include "ofxGui.h"
#include "ofMain.h"

class UIManager {
private:
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    void setupStageParameters();
    void drawInfo();
    ofTrueTypeFont font;
    int fontsize;
public:
    UIManager(){
        setupStageParameters();
    }
    bool showGui;
    ofParameter<bool> kinectDebug;
    ofParameter<int> brushMode;
    ofParameter<string> brushModeLabel;
    ofParameter<bool> showInfo;
    ofParameter<bool> enableKaleidoscope;
    ofParameter<bool> enableKinect;
    ofParameter<bool> enableMovingFbo;
    ofParameter<bool> showOnionSkin;
    ofParameter<int> onionSkinAlpha;
    ofParameter<int> releaseMode;
    ofParameter<int> frameRate;
    ofParameter<bool> enableMouse; // mouse and tablet has a different resolution!
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;
    ofParameter<ofColor> bgColor;

    void setup();
    void draw();
    int getWidth();
    void add(ofParameterGroup _parameters);
    void expandNext ();
    void expandPrev ();
};
