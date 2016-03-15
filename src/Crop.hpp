//
//  Crop.hpp
//  DrawingTool
//
//  Created by Zerc on 12/13/15.
//
//

#ifndef Crop_hpp
#define Crop_hpp

#include "ofMain.h"
class Crop{
private:
public:
    int top, bottom, left, right;
    ofFbo croppedFbo;

    void crop(ofFbo *fbo){
        int w = ofGetWidth();
        int h = ofGetHeight();
        ofPixels pixels;
        pixels.allocate(w, h, GL_RGBA);
        fbo->readToPixels(pixels);
        // top
        for(auto line: pixels.getLines()){
            bool isStop = false;
            for(auto pixel: line.getPixels()){
                if ((int)pixel[3]>0) {
                    top = line.getLineNum();
                    isStop = true;
                    break;
                }
            }
            if(isStop) break;
        }
        // bottom
        auto line = pixels.getLines().end();
        line--;
        for(;line != pixels.getLines().begin(); --line){
            bool isStop = false;
            for(auto pixel: line.getPixels()){
                if (pixel[0]!=0) {
                    bottom = line.getLineNum()+1;
                    isStop = true;
                    break;
                }
            }
            if(isStop) break;
        }
        // left
        int row = 0;
        for (unsigned int i = 0; i < w; i++) {
            bool isStop = false;
            for(auto line = pixels.getLine(top); line < pixels.getLine(bottom); ++line){
                auto pixel = line.getPixels().begin()+row;
                if ((int)pixel[3]>0) {
                    left = row;
                    isStop = true;
                    break;
                }
                if(line.getLineNum()==bottom-1) {
                    row++;
                }
            }
            if(isStop) break;
        }
        // right
        row = w-1;
        for (unsigned int i = w-1; i != 0; i--) {
            bool isStop = false;
            for(auto line = pixels.getLine(top); line < pixels.getLine(bottom); ++line){
                static int counter = 0;
                auto p = line.getPixel(row);
                if ((int)p[3]>0) {
                    right = row+1;
                    isStop = true;
                    break;
                }
                if(line.getLineNum()==bottom-1) {
                    row--;
                }
                counter++;
            }
            if(isStop) break;
        }
        
        int croppedH = bottom - top;
        int croppedW = right - left;

        croppedFbo.allocate(croppedW,croppedH,GL_RGBA);
        croppedFbo.begin();
        ofClear(0, 0);
        ofSetColor(255, 255);
        fbo->getTexture().drawSubsection(0,0,croppedW,croppedH,left ,top, croppedW, croppedH);
        croppedFbo.end();
    };
};

#endif /* Crop_hpp */
