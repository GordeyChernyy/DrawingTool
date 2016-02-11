//
//  AnimatedGif.cpp
//  DrawingTool
//
//  Created by S l on 11/14/15.
//
//

#include "AnimatedGif.h"


//path may be absolute or relative to bin/data
// caller should ALWAYS check the return value to make sure the gif loaded correctly
bool AnimatedGif::loadGif(string path) {
    if(name == path) {
        return true;
    }
    ofDirectory dir(path);
    dir.listDir();
    
    // check to make sure there are images in the directory
    if (dir.size() == 0) {
        // this should never happen
        cout << "Error in animated_gif::setup for " << path << endl;
        return false;
    }
    
    //load all images in the directory
    for(int i = 0; i < dir.size(); i++){
        ofImage *tmp = new ofImage;
        tmp->loadImage(dir[i]);
        if (0 == i) {
            _height = tmp->getHeight();
            _width = tmp->getWidth();
        }
        _imgs.push_back(*tmp);
        
    }
    
    // move anchore from top left corner to middle of image
    _x_anchor = 0.0;//_width / 2.0;
    _y_anchor = 0.0;//_height / 2.0;
    
    // initially set the slant to 0
    _angle = 0.0;
    
    name.assign(path);
    return true;
}

void AnimatedGif::draw(int x, int y, int frame_num) {
    
    if(frame_num >= _imgs.size()) {
        // this should never happen
        cout << "Set a frame number that is too high!  Clamping to last frame" << endl;
        //debug_err();
    }
    
    // incase they send a frame num that's out of bounds, clamp it
    frame_num = ofClamp(frame_num, 0, _imgs.size() - 1);

    // TODO: Make it so it checks the blend state before changing it, so it can set it back at the end
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    _imgs[frame_num].setAnchorPercent(_x_anchor, _y_anchor);
    ofTranslate(x, y);
    ofRotate(_angle);
    ofScale(_scale, _scale);
    _imgs[frame_num].draw(0,0);
    ofPopMatrix();
    
}
