//
//  Font.h
//  DrawingTool
//
//  Created by Zerc on 11/25/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxFTGLFont.h"
class Font {
private:
    ofxFTGLFont font;
public:
    void setup(string path, int size){
        font.loadFont(path);
        font.setSize(size);
    }
    void draw(string content, int x, int y){
        font.drawString(content, x, y);
    }
    int getWidth(string content){
        return font.getWidth(content);
    }
};