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
    ofVec2f *triCoord;
    ofFbo canvas;
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
        setupFbo(canvas);
        triCoord = new ofVec2f[3];
    }
    void update(int x, int y, ofColor col){
        ofVec2f p(x,y);
        int s = history.size();
        if(s<4) addPoint(history, p);
        if(s>3){
//            ofVec2f *points;
//            points = new ofVec2f[2];
//            closestPoint2(p, points, history);
//            triCoord[0] = p;
//            triCoord[1] = points[0];
//            triCoord[2] = points[1];
//            if(points[0].distance(p)>ofRandom(dotDistance)){
//                addPoint(history, p);
//                drawToCanvas(col);
//            }
            vector<ofVec2f> points;
            points = sortClosest(p, history);
            int range = indexRange;
            int min = points.size()-range; // random index min
            int max = points.size()-1; // random index max
            if(points.size() < range){ // avoid negative value
                min = 0;
            }
            int index = (int)ofRandom(min, max);
            int index2 = (int)ofRandom(min, max);
            triCoord[0] = p;
            triCoord[1] = points[index];
            triCoord[2] = points[index2];
            if(points[0].distance(p)>dotDistance*pressure){
                addPoint(history, p);
                drawToCanvas(col);
            }
        }
    }
    void setPressure(float p){
        pressure = p;
    }
    void eraseFirst(vector<ofVec2f> &array){
        array.erase(array.begin());
    }
    void draw(){
        ofSetColor(255, 255);
        canvas.draw(0, 0);
//        debugArray(history);
        ofFill();
        ofSetColor(255, 20);
        drawTriangle(triCoord);
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
    void drawToCanvas(ofColor col){
        canvas.begin();
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(col, opacity);
        drawTriangle(triCoord);
        ofDisableAlphaBlending();
        canvas.end();
    }
    void setupFbo(ofFbo &fbo){
        fbo.allocate(ofGetWidth(), ofGetHeight());
        fbo.begin(); ofClear(0, 0); fbo.end();
    }
    void clear(){
        setupFbo(canvas);
    }
    ofVec2f closestPoint(ofVec2f p){
        return closestPoint(p, history);
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
        if(pos.size()==1){ // need two points at least
            pos.push_back(pos[0]);
        }
        return pos;
    }
    void closestPoint2(ofVec2f p, ofVec2f *&points, vector<ofVec2f> coords){
        float maxDist = ofGetWidth();
        float maxDist2 = ofGetWidth();
        int closestIndex = 0;
        int closestIndex2 = 0;
        for (int i = 0; i < coords.size(); i++) {
            float dist = p.distance(coords[i]);
            closestIndex2 = closestIndex;
            if (dist < maxDist){
                maxDist = dist;
                closestIndex = i;
            }
        }
        for (int i = 0; i<coords.size(); i++) {
            float dist = p.distance(coords[i]);
            if (i != closestIndex && dist < maxDist2){
                maxDist2 = dist;
                closestIndex2 = i;
            }
        }
        points[0] = coords[closestIndex];
        points[1] = coords[closestIndex2];
    }
    ofVec2f closestPoint(ofVec2f p, vector<ofVec2f> coords){
        float maxDist = ofGetWidth();
        int closestIndex = 0;
        for (int i = 0; i<coords.size(); i++) {
            float dist = p.distance(coords[i]);
            if(dist < maxDist){
                maxDist = dist;
                closestIndex = i;
            }
        }
        return coords[closestIndex];
    };

};
