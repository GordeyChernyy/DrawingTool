//
//  Crop.cpp
//  DrawingTool
//
//  Created by Zerc on 12/13/15.
//
//

#include "Crop.hpp"
void Crop::start(ofFbo *fbo){
    fbo->readToPixels(pixels);
    int w = fbo->getWidth();
    int h = fbo->getHeight();

    for(auto line: pixels.getLines()){
        for(auto pixel: line.getPixels()){
            pixel[0]; // red channel on any image
            if(line.getLineNum()>0){
                auto pixelAbove =  pixel - pixels.getWidth();
                // get the pixel 1 line above this one
            }
        }
    }

}