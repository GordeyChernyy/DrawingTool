//
//  AnimatedGif.h
//  DrawingTool
//
//  Created by S l on 11/14/15.
//
//


#include "ofMain.h"
#pragma once

class AnimatedGif {
public:
    bool loadGif(string path);
    void draw(int x, int y,  int frame_num);
    void setScale(float newval) { _scale = newval; }
    void setAnchorPercent(float x, float y) {_x_anchor = x; _y_anchor = y;}
    float getHeight() { return _height; }
    float getWidth() { return _width; }
    void setAngleInDegrees(float angle) {_angle = angle; }
    int getNumFrames() { return _imgs.size(); }
    string *getLoadedGifName() {return &name; }
    
private:
    std::vector<ofImage> _imgs;
    float _x_anchor, _y_anchor;
    float _width, _height;
    float _angle;
    float _scale;
    string name;
};
