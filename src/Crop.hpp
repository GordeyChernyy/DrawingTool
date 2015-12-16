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
    ofPixels pixels;
    ofVec2f newPos;
    ofFbo fbo;
    int minX, minY, maxX, maxY;
public:
    void start(ofFbo *fbo);
    ofFbo getFbo();
    ofVec2f getPos();
};

#endif /* Crop_hpp */
