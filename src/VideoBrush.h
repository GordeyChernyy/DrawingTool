//
//  VideoBrush.h
//  DrawingTool
//
//  Created by S l on 10/28/15.
//
//

#pragma once
#include "ofMain.h"

class VideoBrush{
    public:
        void clear();
        void clearHistory(); // remove all elements from history
        void cleanHistory(); // resize history
        void setup();
        void drawToCanvas(float x, float y, float pressure); // draw to fbo located in this class
        void drawBrush(float x, float y, float pressure); // draw lines
        
        void updateCanvas(ofFbo *fbo, float x, float y, float pressure); // pass fbo
        void draw(); // draw fbo located in this class
        void changeColor(int num);
        void changeSwatch(int &swatch);
        void changeBrush(int &brushType);
        void resize();
        void begin();
        void end();
        ofFbo canvas;
        ofParameterGroup parameters;
        ofParameter<int> size;
        ofParameter<int> brushType;
        ofParameter<string> brushTypeS;
        ofParameter<int> historyMax;
        ofParameter<float> opacity;
        ofParameter<float> lineWidth;
        ofParameter<int> swatch;
        ofParameter<ofColor> bgColor;
        ofParameter<ofColor> activeColor;
        ofParameter<ofColor> col1;
        ofParameter<ofColor> col2;
        ofParameter<ofColor> col3;
        
        vector<ofVec2f> history;
};
