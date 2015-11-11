//
//  TriangleBrush.h
//  DrawingTool
//
//  Created by Zerc on 10/3/15.
//
//

#pragma once
#include "ofMain.h"

class TriangleBrush{
public:
    TriangleBrush(){}
    vector<ofVec2f> history;
    vector<ofVec2f> temp;
    ofVec2f *trianglePoints;
    float pressure;
    ofParameterGroup parameters;
    ofParameter<float> opacity;
    ofParameter<int> dotDistance;
    ofParameter<int> pointChoice;
    ofParameter<int> indexRange;

    void setup(){
        pressure = 1.;
        parameters.setName("TriangleBrush");
        parameters.add(dotDistance.set("dotDistance", 50, 0, 700));
        parameters.add(pointChoice.set("pointChoice", 50, 0, 700));
        parameters.add(indexRange.set("indexRange", 5, 1, 20));
        parameters.add(opacity.set("opacity", 120, 0, 255));
        trianglePoints = new ofVec2f[3];
    }
    void updateCanvas(ofFbo *fbo, int x, int y, ofColor col){
        ofVec2f currentPoint(x,y);
        if (history.size() < 4) addPoint(history, currentPoint);
        if (history.size() > 3){
            vector<ofVec2f> points;
            points = sortClosest(currentPoint, history);   // sort array: last point in array is closest
            setTrianglePoints(points, currentPoint);
            int index = points.size()-1;
            ofVec2f a = points[index];
            ofVec2f b = currentPoint;
            float dist = dotDistance*pressure;
            if(isDistanceBigger(a, b, dist)){
                addPoint(history, currentPoint);
                drawToCanvas(fbo, col);
            }
        }
    }
    void draw(){
        ofFill();
        ofSetColor(255, 20);
        drawTriangle(trianglePoints);
    }
    bool isDistanceBigger(ofVec2f a, ofVec2f b, float distance){
        return a.distance(b) > distance? true : false;
    }
    void setTrianglePoints(vector<ofVec2f> sortedCoord, ofVec2f currentPoint){
        // create random acces to points in sorted array
        // [ . . . . . . ]
        //       |   | |
        //      min    max
        if(sortedCoord.size()==1){   // avoid zero bug
            sortedCoord.push_back(sortedCoord[0]);
        }
        int range = indexRange;
        int min = sortedCoord.size()-range;  // random index min
        int max = sortedCoord.size()-1;  // random index max
        if(sortedCoord.size() < range){  // avoid negative value
            min = 0;
        }
        int index1 = (int)ofRandom(min, max);
        int index2 = (int)ofRandom(min, max);
        trianglePoints[0] = currentPoint;
        trianglePoints[1] = sortedCoord[index1];
        trianglePoints[2] = sortedCoord[index2];
    }
    void setPressure(float p){
        pressure = p;
    }
    void eraseFirst(vector<ofVec2f> &array){
        array.erase(array.begin());
    }

    void drawTriangle(ofVec2f *c){
        ofTriangle(c[0], c[1], c[2]);
    }
    void debugArray(vector<ofVec2f> array){
        for (int i = 0; i<array.size(); i++) {
            ofSetColor(255, 0, 0);
            ofCircle(array[i], 5);
        }
    }
    void addPoint(vector<ofVec2f> &array, ofVec2f pos){
        array.push_back(pos);
    }
    void addPoint(ofVec2f pos){
        addPoint(history, pos);
    }
    void clearHistory(){
        history.clear();
    }
    bool checkAngle(ofVec2f p1, ofVec2f p2, ofVec2f p3){
        float dist1 = p1.distance(p2);
        float dist2 = p1.distance(p3);
        if (dist2 < dist1) {
            return true;
        }else{
            return false;
        }
    }
    void drawToCanvas(ofFbo *fbo, ofColor col){
        fbo->begin();
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        ofFill();
        ofSetColor(col, opacity);
        drawTriangle(trianglePoints);
        glDisable(GL_BLEND);
        fbo->end();
    }
    void drawGraphics(ofColor col){
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(col, opacity);
        drawTriangle(trianglePoints);
        ofDisableAlphaBlending();
    }
    vector<ofVec2f> sortClosest(ofVec2f p, vector<ofVec2f> coords){
        vector<ofVec2f> pos;
        float maxDist = ofGetWidth();
        int closestIndex = 0;
        for (int i = 0; i < coords.size(); i++) {
            float dist = p.distance(coords[i]);
            if (dist < maxDist){
                maxDist = dist;
                pos.push_back(coords[i]);
            }
        }
        return pos;
    }
};
