//
//  TransformUtility.h
//  DrawingTool
//
//  Created by Zerc on 11/30/15.
//
//
#include "ofMain.h"
#pragma once

class TransformUtility {
private:
    ofVec2f pivot, position;
    float angle, angleOffset;
    float scale;
public:
    TransformUtility (){
        angleOffset = 90;
        angle = 0;
        pivot = ofVec2f(0, 0);
        position = ofVec2f(0, 0);
        scale = 1.0;
    }
    void setAngle (float _angle){
        angle = _angle;
    }
    void setAngleOffset (float _angleOffset){
        angleOffset = _angleOffset;
    }
    void setPivot (ofVec2f _pivot){
        pivot = _pivot;
    }
    void setPosition (ofVec2f _position){
        position = _position;
    }
    void setScale (float _scale){
        scale = _scale;
    }
    void begin(){
        ofPushMatrix();
        ofTranslate (pivot);
        ofTranslate (position-pivot);
        ofRotate (angle+angleOffset);
        ofScale (scale, scale);
        ofTranslate (-pivot);
    }
    void end(){
        ofPopMatrix();
    }
    float getAngle (ofVec2f startPoint, ofVec2f endPoint){
        ofVec2f zero(0,1);
        return zero.angle(endPoint-startPoint);
    }
};