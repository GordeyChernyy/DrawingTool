//
//  ParameterManager.h
//  DrawingTool
//
//  Created by Zerc on 11/25/15.
//
//
#pragma once
#include "ofxGui.h"
#include "ofMain.h"

class ParameterManager {
private:
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    
    void setupStageParameters(){
        stageParam.setName("stageParam");
        stageParam.add(brushMode.set("brushMode", 0, 0, 2));
        stageParam.add(brushModeLabel.set("burshmodelabel", "hidude"));
        stageParam.add(showInfo.set("showInfo", true));
        stageParam.add(enableMouse.set("enableMouse", true));
        stageParam.add(enableKaleidoscope.set("enableKaleidoscope", false));
        stageParam.add(enableMovingFbo.set("enableMovingFbo", false));
        stageParam.add(showOnionSkin.set("showOnionSkin", false));
        stageParam.add(onionSkinAlpha.set("onionSkinAlpha", 20, 0, 255));
        stageParam.add(releaseMode.set("releaseMode", 0));
        stageParam.add(pointerSize.set("pointerSize", 5., 0., 10.));
        stageParam.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
        stageParam.add(bgColor.set("bgColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
        parameters.add(stageParam);
    }
    
public:
    ParameterManager(){
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

    void setup (){
        gui.setup(parameters);
        currentParameter++;
        gui.loadFromFile("settings.xml");
        gui.minimizeAll();
        
        vector<string> names = gui.getControlNames();
        // set gui headers color
        for (int i = 0; i<gui.getNumControls(); i++) {
            ofColor col = ofColor(0, 162, 208);
            gui.getGroup(names[i]).setHeaderBackgroundColor(col);
            gui.getGroup(names[i]).setTextColor(ofColor(0));
            gui.getGroup(names[i]).setBorderColor(col);
        }
        showGui = true;
    }
    void draw (){
        gui.draw();
    }
    int getWidth(){
        return gui.getWidth();
    }
    void add(ofParameterGroup _parameters){
        parameters.add(_parameters);
    }
    void expandNext (){
        currentParameter++;
        if (currentParameter>gui.getNumControls()-1) {
            currentParameter = 0;
        }
        vector<string> names = gui.getControlNames();
        gui.minimizeAll();
        gui.getGroup(names[currentParameter]).maximize();
        cout <<  "name " << names[currentParameter] << endl;
    }
    void expandPrev (){
        currentParameter--;
        if (currentParameter<0) {
            currentParameter = gui.getNumControls()-1;
        };
        vector<string> names = gui.getControlNames();
        gui.minimizeAll();
        gui.getGroup(names[currentParameter]).maximize();
        cout <<  "name " << names[currentParameter] << endl;
    }
    
};
