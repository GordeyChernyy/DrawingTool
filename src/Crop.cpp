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

//    for(int y=h-1; y>=0; y--){
//        for(int x=0;x<w;x++){
//            pixels[x*3+y*w*3]; // red channel on an rgb image
//            if(y>0){
//                pixels[x*3+(y-1)*w*3];
//                // get the pixel 1 line above this one
//            }
//        }
//    }
}