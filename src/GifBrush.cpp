//
//  gif.cpp
//  DrawingTool
//
//  Created by S l on 11/11/15.
//
//

#include "GifBrush.h"

void GifBrush::setup(){
    parameters.setName("GifBrush");
    parameters.add(historyMax.set("historyMax", 2000, 100, 10000));
    parameters.add(brushType.set("type line", 0, 0, 1));
    parameters.add(size.set("size", 300, 0, 800));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(frameRate.set("frame rate", 5, 0, 30));

    brushType.addListener(this,&GifBrush::changeGif);
    _bInitialized = false;
    
    // TODO Fix the shader
    //shader.load("shaderVert.c", "shaderFrag.c");
}

void GifBrush::changeGif(int &gifNum){
    switch (gifNum) {
        case 0:
            brushType.setName("rose");
            break;
        case 1:
            brushType.setName("mouse");
            break;
        default:
            break;
    }
}

void GifBrush::draw(){
    if(!_bInitialized) {
        return;
    }
    ofFill();
    ofSetColor(255, 20);
    drawGif(&_start, &_end, FIRST_FRAME);
}

void GifBrush::writeToFbo(ofFbo *fbo, int frame_num) {

    fbo->begin();
    
        ofSetColor(ofColor::white);
        ofFill();
        drawGif(&_start, &_end, frame_num);
    fbo->end();
}

void GifBrush::loadGif(string dir) {
    if(!_gif.loadGif(dir)) {
        //cout << "Couldn't initialize gif!" << endl;
        return;
    }
    _width = _gif.getWidth();
    _height = _gif.getWidth();
    _gif.setAnchorPercent(.5, 1.0);
    _bInitialized = true;
}

void GifBrush::drawGif(ofVec2f *begin, ofVec2f *end, int gifFrame) {
    float angleInDegrees = 0.0;

    // resize image
    float dist = ofDist(begin->x, begin->y , end->x, end->y);
    float growth_factor = dist / _height;
 
    // calculate slant
    angleInDegrees = atan2((begin->y - end->y), (begin->x - end->x)) * 180.0 / PI;
    angleInDegrees += 270.0;
    cout << angleInDegrees << " is degrees." << endl;
    
    _gif.setScale(growth_factor);
    _gif.setAngleInDegrees(angleInDegrees);
    _gif.draw(begin->x, begin->y, gifFrame);
}

void GifBrush::updateCanvas(Timeline *timeline, ofFbo *fbo, float x, float y, float pressure, ofColor col) {
    setEndPosition(x, y);
    draw();
}

void GifBrush::onMouseDown(Timeline *timeline, ofFbo *fbo, int x, int y) {
    loadGif("mouse"); //TODO: Change this to get gif name from drop down menu
    setStartPosition(x, y);
}

void GifBrush::onMouseUp(Timeline *timeline, ofFbo *fbo, int x, int y) {
    int numGifFrames = getNumFramesInGif();
    setEndPosition(x,y);
    
    timeline->setInPointToCurrent();
    // Loop through the next frames of the timeline adding the gif's frames to the timeline
    for(int frameNum = 0; frameNum < numGifFrames; frameNum++) {
        writeToFbo(fbo, frameNum);
        // add as many frames as we need to the timeline so the gif can be fully loaded
        // TODO: Check to make sure with the clips concept this is still implemented correctly
        if(timeline->getCurFrameNum() == timeline->getFrameCount() - 1) {
            timeline->addFrameNextTo();
        }
        timeline->setCurFrame(1, RELATIVE);
        fbo = timeline->getCurFbo();
    }
    
    timeline->setOutPointToCurrent();
    // restore cur frame to the first frame and set it as the start frame
    timeline->setCurFrame(-numGifFrames, RELATIVE);
    setInitialized(false);
    
}

string GifBrush::name() {
    return GIFBRUSHNAME;
}

