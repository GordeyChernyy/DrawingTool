//
//  DreamBrush.h
//  DrawingTool
//
//  Created by Zerc on 8/6/15.
//
//
#include "ofMain.h"
#include "BrushBase.h"
#pragma once

class DreamBrush : public BrushBase {
public:
    DreamBrush(){
        setup();
    }
    void clear();
    void clearHistory(); // remove all elements from history
    void cleanHistory(); // resize history 
    void setup();
    void onMouseUp(int x, int y);
    void onMouseDown(int x, int y);
    void drawToCanvas(float x, float y, float pressure); // draw to fbo located in this class
    void drawBrush(float x, float y, float pressure, ofColor color); // draw lines
    
    void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col); // pass fbo
    void draw(); // draw fbo located in this class
    void changeBrush(int &brushType);
    void resize();
    void begin();
    void end();
    string name();
    ofFbo canvas;
    ofParameter<int> size;
    ofParameter<int> brushType;
    ofParameter<string> brushTypeS;
    ofParameter<int> historyMax;
    ofParameter<float> opacity;
    ofParameter<float> lineWidth;
   
    vector<ofVec2f> history;
};
