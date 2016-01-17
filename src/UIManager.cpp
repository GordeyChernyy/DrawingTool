//
//  UIManager.cpp
//  DrawingTool
//
//  Created by Zerc on 11/25/15.
//
//

#include "UIManager.h"
void UIManager::setupStageParameters(){
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
    fontsize = 9;
    font.load("CourierNew.ttf", fontsize);
}
void UIManager::setup(){
    gui.setup(parameters);
    currentParameter++;
    gui.loadFromFile("settings.xml");
    gui.minimizeAll();
    
    vector<string> names = gui.getControlNames();
    // set gui headers color
    for (int i = 0; i<gui.getNumControls(); i++) {
        cout <<  "gui name = " << names[i]  << endl;
        ofColor col = ofColor(0, 162, 208);
        gui.getGroup(names[i]).setHeaderBackgroundColor(col);
        gui.getGroup(names[i]).setTextColor(ofColor(0));
        gui.getGroup(names[i]).setBorderColor(col);
    }
    showGui = true;
}
void UIManager::draw(){
    if(showGui) {
        gui.draw();
        drawInfo();
    }
}
void UIManager::drawInfo(){
    if (showInfo) {
        vector<string> s;
        s.push_back("-- KEY ----------------------------------");
        s.push_back("1, 2, 3, 4    change colors");
        s.push_back("TAB           hide gui");
        s.push_back("q, w          cycle through parameters");
        s.push_back("SPACE         play image");
        s.push_back("s             create new frame");
        s.push_back("f             next frame");
        s.push_back("d             previous frame");
        s.push_back("e             triangle brush");
        s.push_back(",             dream catcher brush");
        s.push_back("-             clear image");
        s.push_back("x             releaseMode: ");
        s.push_back("x             press first time: auto cre-");
        s.push_back("              ate mode on pen release");
        s.push_back("x             press second time: auto");
        s.push_back("              next frame");
        s.push_back("x             third time: return to defau");
        s.push_back("              lt.");
        s.push_back("-- INFO ---------------------------------");
        s.push_back("frame rate: " + ofToString(ofGetFrameRate()));
        s.push_back("-- TIPS ---------------------------------");
        s.push_back("It looks interesting if you change color");
        s.push_back("and draw at the same time.");
        s.push_back("Kaleidoscope is still in development. Po-");
        s.push_back("sition of drawing is not matching. Try ");
        s.push_back("to draw more on the left top corner.");
        int width = 280;
        int border = 20;
        int x = ofGetWidth()-width-border*2;
        int y = 20;
        int lineHeight = 13;
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(255,120);
        ofDrawRectangle(x-border, y-fontsize-border, width+border*2, lineHeight*(s.size()-1)+border*2);
        ofSetColor(0, 255);
        for (int i = 0; i < s.size()-1; i++) {
            font.drawString(s[i], x, y+lineHeight*i);
        }
        ofDisableAlphaBlending();
    }
}
int UIManager::getWidth(){
    return gui.getWidth();
}
void UIManager::add(ofParameterGroup _parameters){
    parameters.add(_parameters);
}
void UIManager::expandNext(){
    currentParameter++;
    if (currentParameter>gui.getNumControls()-1) {
        currentParameter = 0;
    }
    vector<string> names = gui.getControlNames();
    gui.minimizeAll();
    gui.getGroup(names[currentParameter]).maximize();
    cout <<  "name " << names[currentParameter] << endl;
}
void UIManager::expandPrev(){
    currentParameter--;
    if (currentParameter<0) {
        currentParameter = gui.getNumControls()-1;
    };
    vector<string> names = gui.getControlNames();
    gui.minimizeAll();
    gui.getGroup(names[currentParameter]).maximize();
    cout <<  "name " << names[currentParameter] << endl;
}