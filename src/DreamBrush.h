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
    void clear();
    void clearHistory(); // remove all elements from history
    void cleanHistory(); // resize history 
    void setup();
    void drawToCanvas(float x, float y, float pressure); // draw to fbo located in this class
    void drawBrush(float x, float y, float pressure); // draw lines
    
    void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col); // pass fbo
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
    
    ofColor swatches[13][4] = {
        {
            ofColor(110,255,2),
            ofColor(235,255,19),
            ofColor(255,9,79),
            ofColor(8,10,10)},
        {
            ofColor(211,253,253),
            ofColor(197,255,48),
            ofColor(231,83,49),
            ofColor(214,9,44)
        },
        {
            ofColor(254,5,12),
            ofColor(138,4,109),
            ofColor(254,129,253),
            ofColor(8,10,11)
        },
        {
            ofColor(244,255,143),
            ofColor(173,9,70),
            ofColor(242,250,1),
            ofColor(189,199,219)
        },
        {
            ofColor(57,150,254),
            ofColor(147,83,255),
            ofColor(254,26,14),
            ofColor(243,255,133)
        },
        {
            ofColor(250,23,30),
            ofColor(29,136,82),
            ofColor(243,250,39),
            ofColor(235,145,24)
        },
        {
            ofColor(184,145,252),
            ofColor(195,64,235),
            ofColor(240,145,33),
            ofColor(212,171,138)
        },
        {
            ofColor(85,193,144),
            ofColor(240,255,92),
            ofColor(187,89,68),
            ofColor(193,188,140)
        },
        {
            ofColor(234,255,10),
            ofColor(254,3,15),
            ofColor(254,184,224),
            ofColor(253,178,94)
        },
        {
            ofColor(240,255,11),
            ofColor(167,255,2),
            ofColor(193,147,254),
            ofColor(246,1,11)
        },
        {
            ofColor(236,0,22),
            ofColor(227,61,241),
            ofColor(219,128,190),
            ofColor(208,120,254)
        },
        {
            ofColor(206,206,194),
            ofColor(254,3,66),
            ofColor(119,62,255),
            ofColor(168,80,57)
        },
        {
            ofColor(87,202,140),
            ofColor(236,255,24),
            ofColor(173,236,148),
            ofColor(194,61,176)
        },
        
    };
};
