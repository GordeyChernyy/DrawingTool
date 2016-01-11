//
//  TriangleBrush.h
//  DrawingTool
//
//  Created by Zerc on 10/3/15.
//
//

#pragma once
#include "ofMain.h"
#include "BrushBase.h"

class TriangleBrush : public BrushBase {
public:
    TriangleBrush(){
        setup();
    }
    vector<ofVec2f> history;
    vector<ofVec2f> temp;
    ofVec2f *trianglePoints;
    float pressure;
    ofParameter<float> opacity;
    ofParameter<int> dotDistance;
    ofParameter<int> pointChoice;
    ofParameter<int> indexRange;

    void setup();
    void onMouseUp(int x, int y);
    void onMouseDown(int x, int y);
    void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col);
    void draw();
    bool isDistanceBigger(ofVec2f a, ofVec2f b, float distance);
    void setTrianglePoints(vector<ofVec2f> sortedCoord, ofVec2f currentPoint);
    void setPressure(float p);
    void eraseFirst(vector<ofVec2f> &array);
    void drawTriangle(ofVec2f *c);
    void debugArray(vector<ofVec2f> array);
    void addPoint(vector<ofVec2f> &array, ofVec2f pos);
    void addPoint(ofVec2f pos);
    void clearHistory();
    bool checkAngle(ofVec2f p1, ofVec2f p2, ofVec2f p3);
    void drawToCanvas(ofFbo *fbo, ofColor col);
    void drawGraphics(ofColor col);
    vector<ofVec2f> sortClosest(ofVec2f p, vector<ofVec2f> coords);
    string name();
};
