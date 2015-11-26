#include "timeline_types.h"

Timeline::Timeline() {
}

void Timeline::setup(int x, int y, int width, int height) {
    
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    // TODO: Get rid of _frame_width and _frame_height?
    _frame_width = ofGetWindowWidth();
    _frame_height = ofGetWindowHeight();
    
    _frames.reserve(RESERVE_FRAMES);
    Frame *frame = new Frame;
    frame->setup(_frame_width, _frame_height);
    _frames.push_back(*frame);
    
    _cur_layer = 0;     // count from 0
    _num_layers = 1;    // count from 1
    _cur_frame = 0;    // count from 0
    curPlayFrame = 0;
    
    /* Test max buffers that can be allocated
    test *cur = &_fbo_test;
    
    for(int i = 0 ; i < 360; i++) {
        cur->fbo = (unsigned char*)malloc(width * height * 4);
        cur->next = (test *)malloc(sizeof(test));
        cur = cur->next;
    } */
    
    
    _bPlaying = false;
    frameRate = 3;
    _start_frame = 0;
    _stop_frame = 0;
    
    return;
}
void Timeline::setFrameRate(int _frameRate){
    frameRate = _frameRate;
}
int Timeline::getCurFrameNum() {
    return _cur_frame;
}

// returns Frames vector
std::vector<Frame> *Timeline::getFrames() {
    return &_frames;
}

// returns number of frames
int Timeline::getNumFrames() {
    return _frames.size();
}
// adds a frame either at _cur_frame.  Does not change  _cur_frame
void Timeline::addFrame() {
    Frame *new_frame = new Frame;
    new_frame->setup(_frame_width, _frame_height);
    _frames.insert(_frames.begin() + _cur_frame + 1, *new_frame);
}

// adds a layer one above the current layer
void Timeline::addLayer() {
    _frames[_cur_frame].addLayer(_frame_width, _frame_height, _cur_layer + 1);
    _num_layers += 1;
    checkTimelineResize();
}

// makes sure we can always draw all our layers
void Timeline::checkTimelineResize() {
    int max_h = 0;
    max_h = (_num_layers * FRAME_SIZE) + (2 * EDGE_SPACER) + NUM_SPACER;
    while(max_h > _height) {
        _height += FRAME_SIZE;
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
// plays all frames inside _frames. Does not use in out constraints
void Timeline::playFramesDetached(){
    beginBlend();
    _frames[curPlayFrame].draw();
    endBlend();
    if(ofGetFrameNum()%frameRate == 0){
        curPlayFrame += 1;
        if(curPlayFrame > _frames.size()-1) {
            curPlayFrame = 0;
        }
    }
}
// Responsible for drawing the actual FBO for the frame, as opposed to drawing on the timeline
// This function SHOULD be called by the main app
void Timeline::drawCurFrame() {
    
    _frames[_cur_frame].setAlpha(255);
    beginBlend();
    _frames[_cur_frame].draw();
    endBlend();
    if(_bPlaying && ofGetFrameNum()%frameRate == 0) {
        _cur_frame += 1;
        if(_cur_frame > _stop_frame) {
            _cur_frame = _start_frame;
        }
    }
   
}
void Timeline::clearCurFrame(){
   _frames[_cur_frame].clear();
}
void Timeline::drawOnionSkin(int alpha){
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    int frameBefore = _cur_frame-1;
    int frameAfter = _cur_frame+1;
    if (frameBefore >= 0) {
        _frames[frameBefore].setAlpha(alpha);
        _frames[frameBefore].draw();
    }
    if (frameAfter <= _frames.size()-1) {
        _frames[frameAfter].setAlpha(20);
        _frames[frameAfter].draw();
    }
    glDisable(GL_BLEND);
}
// Draws a number line with the frame numbers
// This function returns immediately except 1 out of FRAME_NUM_GRANULARITY times
void Timeline::drawFrameNum(int x, int frame_num) {
    if (frame_num % FRAME_NUM_GRANULARITY != 0) {
        return;
    }
    int y = _y + _height - NUM_SPACER/3; //TODO: This is a hack becaue it looked right, make this based on FRAME_SIZE
    string s;
    ostringstream convert;
    convert << frame_num;
    s = convert.str();
    ofSetColor(ofColor::black); // shadow
    ofDrawBitmapString(s, x + 1, y + 1);
    ofSetColor(ofColor::peachPuff);
    ofDrawBitmapString(s, x, y);
}

// returns the FBO corresponding to the current frame and current layers
ofFbo *Timeline::getCurFbo() {
    ofFbo *retval = _frames[_cur_frame].getCurFbo(_cur_layer);
    return retval;
}


// Responsible for drawing the timeline, NOT the contents of the frames (use Timeline::drawCurFrame for that)
// We draw in the middle of the timeline the current frame
// We start by drawing cur frame and it's layer in the middle.  We color it to indicate it is cur frame
void Timeline::drawTimeline() {
    //draw background of timeline
    ofSetColor(COLOR_TIMELINE_BRACKGROUND);
    ofRect(_x, _y, _width, _height);

    // calculate what the frame num of the left-most frame and right-most frame
    int pot_right = (_width / 2) / FRAME_SIZE; // potential right. max number of frames that fit to right of middle frame
    int pot_left = (_width / 2) / FRAME_SIZE; // potential left. max number of frames that fit to left of middle frame
    pot_right -= 1;           // potential right. adjust because we don't count cur frame
    
    int act_left = MIN(_cur_frame, pot_left); // actual left. number of frames to draw left
    int act_right = MIN(_frames.size() - _cur_frame - 1, pot_right); // actual right. number of frames to draw right
    int low_frame = _cur_frame - act_left;
    int high_frame = _cur_frame + act_right;
    
    // calculate the coordinates of the middle of the timeline in x dimension, bottom of timeline in y
    int init_x = _x + _width / 2;
    init_x += -((_cur_frame - low_frame) * FRAME_SIZE);  // adjust for whatever the left most frame to be drawn is
    int init_y = _y + _height - EDGE_SPACER - FRAME_SIZE- NUM_SPACER;
    int cur_x = init_x;
    int cur_y = init_y;

    // TODO: Should f be < or <= s
    for(int f = low_frame; f <= high_frame; f++) {
        drawFrameNum(cur_x, f);
        for(int l = 0;  l < _num_layers; l++) {
            // the selected frame should be a different color from all the other frames
            // and the selected layer in that frame should be an even different color
            if((l == _cur_layer) && (f == _cur_frame)) {
                ofSetColor(COLOR_CUR_DRAWSPACE);
            } else if(f == _cur_frame){
                ofSetColor(COLOR_CUR_FRAME);
            } else {
                ofSetColor(COLOR_FRAME);
            }
            
            // First draw a sold rectangle, then border it in black
            ofFill();
            ofRect(cur_x, cur_y, FRAME_SIZE, FRAME_SIZE);
            ofSetColor(ofColor::black);
            ofNoFill();
            ofRect(cur_x, cur_y, FRAME_SIZE, FRAME_SIZE);
            ofFill();
            
            // draw the next layer above the current one
            cur_y -= FRAME_SIZE;
            
            // check to make sure we aren't drawing off of the timeline
            if (cur_y < (_y + EDGE_SPACER + NUM_SPACER)) {
                dbg_error("About to draw off the edge of the timeline!");
            }
        }
        // reset y to the bottom of the timeline, and iterate x to the right by one frame
        cur_y = init_y;
        cur_x += FRAME_SIZE;
        if(cur_x > (_x + _width - EDGE_SPACER - FRAME_SIZE)) {
            dbg_error("About to draw off the edge of the timeline in x direction!");
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
    for(int f = 0; f < _frames.size(); f++) {
        for(int l = 0; l < _frames[f].getLayers()->size(); l++) {
            out += 1;
        }
    }
    return out;
}

// sets the current frame, either as an offset from the current frame, or as an absolute value from frame 0
// TODO: Should we allocate the fbo if all we are doing is steppping over the frame?  Maybe a waste of memory?
void Timeline::setCurFrame(int pos, int method) {
    // determine the new positon and clamp it between 0 and _frame.size() -1
    switch (method) {
        case RELATIVE:
            _cur_frame = ofClamp(_cur_frame + pos, 0, _frames.size() -  1);
            break;
        case ABSOLUTE:
            _cur_frame = ofClamp(pos, 0, _frames.size() - 1);
            break;
        default:
            dbg_error("Invalid method in Timeline::setbCurFrame");
            break;
    }
    //TODO: Allocate the layer/fbo if it doesn't already exist
}

// sets the current layer, either as an offset from the current layer, or
// 	as an absolute value from layer 0
// TODO: Should we allocate the fbo if all we are doing is steppping over the frame? Maybe a waste of memory?
void Timeline::setCurLayer(int pos, int method) {
    // determine the new positon and clamp it between 0 and _num_layers - 2
    switch(method) {
        case RELATIVE:
            _cur_layer = ofClamp(_cur_layer + pos, 0, _num_layers - 1);
            break;
        case ABSOLUTE:
            _cur_layer = ofClamp(pos, 0, _num_layers - 1);
            break;
        default:
            dbg_error("Invalid method in Timeline::setCurLayer" );
            break;
    }
    //TODO: Allocate the fbo if it doesn't already exist
}

// call this if _x, _y, _width, or _height have changed and these values
// will be used to recalculate the other values
void Timeline::windowResize(int w, int h) {
    _frame_width = ofGetWindowWidth(); //could use the arguments to the function but i dont
    _frame_height = ofGetWindowWidth();
    _x = ofGetWindowWidth() * (1.0/8.0);
    _y = ofGetWindowHeight() * (3.0/4.0);
    _width = ofGetWindowWidth() * (3.0/4.0); // width = 75% of screen
    _height = ofGetWindowHeight() * (1.0/5.0); // height == %20 of screen
    
    checkTimelineResize();
    
    // TODO if we resize, should we resize all the fbo's?
    // or should we keep them small and if they try to draw outside of the original region
    // for that layer, it won't work?
    // or should we stretch the fbo's to the new dimension (almost certainly no, but it's an idea)
    return;
}

void Timeline::delCurFrame() {
    int new_cur_frame = 0;
    
    if(_frames.size() == 1) {
        cout << "Can't delete the only frame!" << endl;
        return;
    }
    
    // check if the cur_frame is the last frame
    // -1 from frame.size() because _cur_frame counts from 0 and _frame.size() counts from 1
    if(_cur_frame == (_frames.size() - 1)) {
        new_cur_frame = _cur_frame - 1;
    }
    else {
        new_cur_frame = _cur_frame;
    }
    
    
    _frames[_cur_frame].destroyLayers();
    _frames.erase(_frames.begin() + _cur_frame);
    
    _cur_frame = new_cur_frame;
}


