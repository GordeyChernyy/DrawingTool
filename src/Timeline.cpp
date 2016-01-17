#include "Timeline.h"

Timeline::Timeline() {
}

void Timeline::setup(int x, int y, int _width, int _height) {
    _x = x;
    _y = y;
    width = _width;
    height = _height;
    // TODO: Get rid of frameWidth and frameHeight?
    frameWidth = ofGetWindowWidth();
    frameHeight = ofGetWindowHeight();
    
    addFrame();
    
    curlayerNum = 0;
    numLayers = 1;
    inPoint = 0;
    outPoint = 0;
    curFrame = 0;
    
    isPlaying = false;
    frameRate = 3;

    font.load("Arial.ttf", 8);
    parameters.setName("Timeline");
    parameters.add(showOnionSkin.set("showOnionSkin", false));
    parameters.add(autoFrameHandleMode.set("autoFrameHandleMode", 0, 0, 2));
    parameters.add(onionSkinAlpha.set("autoFrameHandleMode", 200, 0, 255));
}

Frame *Timeline::getFrame(int num){
    if ((num > frames.size()) || (num < 0)) {
        cout << "Problem in Timeline::getFrame" << endl;
        num = ofClamp(num, 0, frames.size());
    }
    return &frames[num];
}

int Timeline::getNumLayers(){
    return numLayers;
}
int Timeline::getInPoint(){
    return inPoint;
}
int Timeline::getOutPoint(){
    return outPoint;
}
int Timeline::getFrameCount(){
    return frames.size();
}
// returns the FBO corresponding to the current frame and current layers
ofFbo *Timeline::getCurFbo() {
    return frames[curFrame].getCurFbo(curlayerNum);
}

// adds a frame either at currFrame.  Does not change  currFrame
void Timeline::addFrameNextTo() {
    Frame *new_frame = new Frame;
    new_frame->setup(frameWidth, frameHeight);
    frames.insert(frames.begin() + curFrame + 1, *new_frame);
}
void Timeline::addFrame(){
    Frame *new_frame = new Frame;
    new_frame->setup(frameWidth, frameHeight);
    frames.insert(frames.begin(), *new_frame);
}

// adds a layer one above the current layer
void Timeline::addLayer() {
    frames[curFrame].addLayer(frameWidth, frameHeight, curlayerNum + 1);
    numLayers += 1;
    checkTimelineResize();
}

void Timeline::setFrameRate(int _frameRate){
    frameRate = _frameRate;
}
// sets the current frame, either as an offset from the current frame, or as an absolute value from frame 0
// TODO: Should we allocate the fbo if all we are doing is steppping over the frame?  Maybe a waste of memory?
void Timeline::setCurFrame(int pos, int method) {
    // determine the new positon and clamp it between 0 and _frame.size() -1
    switch (method) {
        case RELATIVE:
            curFrame = ofClamp(curFrame + pos, 0, frames.size() -  1);
            break;
        case ABSOLUTE:
            curFrame = ofClamp(pos, 0, frames.size() - 1);
            break;
        default:
            dbg_error("Invalid method in Timeline::setbCurFrame");
            break;
    }
    //TODO: Allocate the layer/fbo if it doesn't already exist
}

// sets the current layer, either as an offset from the current layer, or
// as an absolute value from layer 0
// TODO: Should we allocate the fbo if all we are doing is steppping over the frame? Maybe a waste of memory?
void Timeline::setCurLayer(int pos, int method) {
    // determine the new positon and clamp it between 0 and numLayers - 2
    switch(method) {
        case RELATIVE:
            curlayerNum = ofClamp(curlayerNum + pos, 0, numLayers - 1);
            break;
        case ABSOLUTE:
            curlayerNum = ofClamp(pos, 0, numLayers - 1);
            break;
        default:
            dbg_error("Invalid method in Timeline::setCurLayer" );
            break;
    }
    //TODO: Allocate the fbo if it doesn't already exist
}
void Timeline::setOutPointToCurrent(){
    outPoint = curFrame;
}
void Timeline::setInPointToCurrent(){
    inPoint = curFrame;
}
// makes sure we can always draw all our layers
void Timeline::checkTimelineResize() {
    int max_h = 0;
    max_h = (numLayers * FRAME_SIZE) + (2 * EDGE_SPACER) + NUM_SPACER;
    while(max_h > height) {
        height += FRAME_SIZE;
        _y -= FRAME_SIZE;
    }
}
void Timeline::beginBlend(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
void Timeline::endBlend(){
    glDisable(GL_BLEND);
}
void Timeline::clearCurFrame(){
   frames[curFrame].clear(curlayerNum);
}

void Timeline::delCurFrame() {
    int newcurrFrame = 0;
    
    if(frames.size() == 1) {
        cout << "Can't delete the only frame!" << endl;
        return;
    }
    
    // check if the cur_frame is the last frame
    // -1 from frame.size() because currFrame counts from 0 and _frame.size() counts from 1
    if(curFrame == (frames.size() - 1)) {
        newcurrFrame = curFrame - 1;
    }
    else {
        newcurrFrame = curFrame;
    }
    
    
    frames[curFrame].destroyLayers();
    frames.erase(frames.begin() + curFrame);
    
    curFrame = newcurrFrame;
}
// Responsible for drawing the actual FBO for the frame, as opposed to drawing on the Timeline
// This function SHOULD be called by the main app
void Timeline::drawCurFrame() {
//    cout <<  "count= " << getFrameCount()  << endl;
    frames[curFrame].setAlpha(255);
    beginBlend();
    frames[curFrame].draw();
    endBlend();
    if(isPlaying && ofGetFrameNum()%frameRate == 0) {
        curFrame += 1;
        if(curFrame > getOutPoint()) {
            curFrame = getInPoint();
        }
    }
    
}
void Timeline::drawOnionSkin(int alpha){
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    int frameBefore = curFrame-1;
    int frameAfter = curFrame+1;
    if (frameBefore >= 0) {
        getFrame(frameBefore)->setAlpha(alpha);
        getFrame(frameBefore)->draw();
    }
    if (frameAfter <= frames.size()-1) {
        getFrame(frameBefore)->setAlpha(20);
        getFrame(frameBefore)->draw();
    }
    glDisable(GL_BLEND);
}

// Draws a number line with the frame numbers
// This function returns immediately except 1 out of FRAME_NUM_GRANULARITY times
void Timeline::drawFrameNum(int x, int frame_num) {
    if (frame_num % FRAME_NUM_GRANULARITY != 0) {
        return;
    }
    int y = _y + height-20; //TODO: This is a hack becaue it looked right, make this based on FRAME_SIZE
    string s;
    ostringstream convert;
    convert << frame_num;
    s = convert.str();
    ofSetColor(ofColor::black);
    font.drawString(s, x-2, y);
}

// Responsible for drawing the Timeline, NOT the contents of the frames (use Timeline::drawCurFrame for that)
// We draw in the middle of the Timeline the current frame
// We start by drawing cur frame and it's layer in the middle.  We color it to indicate it is cur frame
void Timeline::drawTimeline() {
    //draw background of Timeline
    ofSetColor(COLOR_TIMELINE_BRACKGROUND);
//    ofDrawRectangle(_x, _y, width, height);

    // calculate what the frame num of the left-most frame and right-most frame
    int pot_right = (width / 2) / FRAME_SIZE; // potential right. max number of frames that fit to right of middle frame
    int pot_left = (width / 2) / FRAME_SIZE; // potential left. max number of frames that fit to left of middle frame
    pot_right -= 1;           // potential right. adjust because we don't count cur frame
    
    int act_left = MIN(curFrame, pot_left); // actual left. number of frames to draw left
    int act_right = MIN(frames.size() - curFrame - 1, pot_right); // actual right. number of frames to draw right
    int low_frame = curFrame - act_left;
    int high_frame = curFrame + act_right;
    
    // calculate the coordinates of the middle of the Timeline in x dimension, bottom of Timeline in y
    int init_x = _x + width / 2;
    init_x += -((curFrame - low_frame) * FRAME_SIZE);  // adjust for whatever the left most frame to be drawn is
    int init_y = _y + height - EDGE_SPACER - FRAME_SIZE- NUM_SPACER;
    int cur_x = init_x;
    int cur_y = init_y;

    // TODO: Should f be < or <= s
    for(int f = low_frame; f <= high_frame; f++) {
        drawFrameNum(cur_x, f);
        for(int l = 0;  l < numLayers; l++) {
            // the selected frame should be a different color from all the other frames
            // and the selected layer in that frame should be an even different color
            if((l == curlayerNum) && (f == curFrame)) {
                ofSetColor(COLOR_CUR_DRAWSPACE);
            } else if(f == curFrame){
                ofSetColor(COLOR_CUR_FRAME);
            } else {
                ofSetColor(COLOR_FRAME);
            }
            
            ofFill();
            ofDrawCircle(cur_x, cur_y, FRAME_SIZE/3, FRAME_SIZE/3);
            ofSetColor(ofColor::black);
            ofFill();
            
            // draw the next layer above the current one
            cur_y -= FRAME_SIZE;
            
            // check to make sure we aren't drawing off of the Timeline
            if (cur_y < (_y + EDGE_SPACER + NUM_SPACER)) {
                dbg_error("About to draw off the edge of the Timeline!");
            }
        }
        // reset y to the bottom of the Timeline, and iterate x to the right by one frame
        cur_y = init_y;
        cur_x += FRAME_SIZE;
        if(cur_x > (_x + width - EDGE_SPACER - FRAME_SIZE)) {
            dbg_error("About to draw off the edge of the Timeline in x direction!");
            break;
        }
    }
}

// when there's an unexpected condition, call this
void dbg_error(string err_msg) {
    cout << err_msg << endl;
    cout << "This shouldn't have happened!" << endl;
}

// helper functions for diagnostics (looking for slow down from fbo over-allocation for example)
int Timeline::countAllocatedFbos() {
    int out = 0;
    for(int f = 0; f < frames.size(); f++) {
        for(int l = 0; l < getFrame(f)->getLayers()->size(); l++) {
            out += 1;
        }
    }
    return out;
}

// call this if _x, _y, width, or height have changed and these values
// will be used to recalculate the other values
void Timeline::windowResize(int w, int h) {
    frameWidth = ofGetWindowWidth(); //could use the arguments to the function but i dont
    frameHeight = ofGetWindowWidth();
    _x = ofGetWindowWidth() * (1.0/8.0);
    _y = ofGetWindowHeight() * (3.0/4.0);
    width = ofGetWindowWidth() * (3.0/4.0); // width = 75% of screen
    height = ofGetWindowHeight() * (1.0/5.0); // height == %20 of screen
    
    checkTimelineResize();
    
    // TODO if we resize, should we resize all the fbo's?
    // or should we keep them small and if they try to draw outside of the original region
    // for that layer, it won't work?
    // or should we stretch the fbo's to the new dimension (almost certainly no, but it's an idea)
    return;
}
void Timeline::draw(){
    drawCurFrame();
    drawTimeline();
}
void Timeline::autoFrameHandleSwitch(){
    autoFrameHandleMode > 1 ? autoFrameHandleMode = 0 : autoFrameHandleMode++;
}
void Timeline::autoFrameHandle(){
    switch (autoFrameHandleMode) {
        case 0:
            break;
        case 1: // auto frame add
            addFrameNextTo();
            setCurFrame(1, RELATIVE);
            setOutPointToCurrent();
            break;
        case 2: // auto next frame
            if(curFrame == getFrameCount()-1){
                setCurFrame(0, ABSOLUTE);
            } else {
                setCurFrame(1, RELATIVE);
            }
            break;
    }
}
void Timeline::addAndMove(){
    addFrameNextTo();
    setCurFrame(1, RELATIVE);
    setOutPointToCurrent();
}
void Timeline::moveLeft(){
    setCurFrame(-1, RELATIVE);
}
void Timeline::moveRight(){
    setCurFrame(1, RELATIVE);
}
void Timeline::playPause(){
    if(getPlaying()) {
        setPlaying(false);
    } else {
        setPlaying(true);
    }
}
