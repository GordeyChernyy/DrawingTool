#include "timeline_types.h"

// TODO: Make it so the FBO's are actually store on the time line and editable
// It's kind of neat having all the GPU junky memory appear, but remeber to initialize FBos'

Timeline::Timeline() {
}

void Timeline::setup(int x, int y, int width, int height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    _frame_width = ofGetWindowWidth();
    _frame_height = ofGetWindowHeight();
    
    _clicks.reserve(RESERVE_CLICKS);
    _bUpdateClickList = true;
    
    _frames.reserve(RESERVE_FRAMES);
    Frame *frame = new Frame;
    frame->setup(_frame_width, _frame_height, true);
    _frames.push_back(*frame);

    if(pthread_mutex_init(&_frame_mutex, NULL)) {
        cout << "frame mutex init failed" << endl;
        dbg_error();
    }
    if(pthread_mutex_init(&_click_mutex, NULL)) {
        cout << "click mutex init failed" << endl;
        dbg_error();
    }
    return;
}

// returns Frames vector
std::vector<Frame> Timeline::getFrames() {
    return _frames;
}

// returns number of frames
int Timeline::getNumFrames() {
    return _frames.size();
}

// adds a frame either at end of timeline or at cur position
void Timeline::addFrame(int method) {
    pthread_mutex_lock(&_frame_mutex);
    
    Frame *new_frame = new Frame;
    new_frame->setup(_frame_width, _frame_height, false);
    switch(method) {
        case RELATIVE: {
            int cur_frame = _getCurFrameNum();
            _frames.insert(_frames.begin() + cur_frame + 1, *new_frame);
            break; }
        case ABSOLUTE: {
            _frames.push_back(*new_frame);
            break;
        }
        default:
            cout << "Invalid method in Timeline::addFrame!" << endl;
            dbg_error();
            break;
    }
    
    _bUpdateClickList = true;
    pthread_mutex_unlock(&_frame_mutex);
}

// adds a layer to the current frame, either at the current layer or at the top of the layers
void Timeline::addLayer(int method) {
    pthread_mutex_lock(&_frame_mutex);
    
    int cur_frame = _getCurFrameNum();
    _frames[cur_frame].addLayer(method, _frame_width, _frame_height);
    
    _bUpdateClickList = true;
    
    pthread_mutex_unlock(&_frame_mutex);
}

// adds an entry to the _clicks array, which MUST be updated under these conditions:
//  1. a frame or layer is added
//  2. a frame or layer is removed
//  3. we change the current frame we are on
// TODO: Should the size field be generalized from being TIMELINE_FRAME_SIZE?
void Timeline::updateClickList(int x, int y, int frame_num, int layer_num) {
    ClickFrame *click = (ClickFrame*)malloc(sizeof(ClickFrame));
    click->x = x;
    click->y = y;
    click->size = TIMELINE_FRAME_SIZE;
    click->frame_num = frame_num;
    click->layer_num = layer_num;
    
    _clicks.push_back(*click);
}

// frees all the entries in the _clicks array
// destroyClickList MUST be updated under these conditions:
//  1. a frame or layer is added
//  2. a frame or layer is removed
//  3. we change the current frame we are on
void Timeline::destroyClickList() {
    while(_clicks.size() > 0) {
        _clicks.pop_back();
    }
}

// Responsible for drawing the contents of the frame, as opposed to drawing on the timeline
// SHOULD be called by the main app
void Timeline::drawCurFrame() {
    pthread_mutex_lock(&_frame_mutex);
    int cur_frame = _getCurFrameNum();
    _frames[cur_frame].draw();
    pthread_mutex_unlock(&_frame_mutex);
}

void Timeline::drawFrameRects(int cur_x, int cur_y, int frame, bool b_cur_frame) {
    for(int l =0 ;  l < _frames[frame].getLayers().size(); l++) {
        // the selected frame should be a different color from all the other frames
        // and the selected layer in that frame should be an even different color
        if((l == _frames[frame].getCurLayer()) && b_cur_frame) {
            ofSetColor(COLOR_CURRENT_DRAWSPACE);
        } else if(b_cur_frame){
            ofSetColor(COLOR_CUR_FRAME);
        } else {
            ofSetColor(COLOR_FRAME);
        }
        ofRect(cur_x, cur_y, TIMELINE_FRAME_SIZE, TIMELINE_FRAME_SIZE);

        // if the timeline changed since we last drew, add the frame to the click list
        if(_bUpdateClickList) {
            updateClickList(cur_x, cur_y, frame, l);
        }

        // draw the next layer above the current one
        cur_y -= (TIMELINE_FRAME_SIZE + TIMELINE_FRAME_SPACER);

        // check to make sure we aren't drawing off of the timeline
        // TODO: Should we just not draw the layers above this?
        if (cur_y < (_y + TIMELINE_EDGE_SPACER + TIMELINE_NUMLINE_SPACER)) {
            break;
        }
    }

}

void Timeline::draw_frame_num(int x, int frame_num) {
    if (frame_num % FRAME_NUMBER_GRANULARITY != 1) {
        return;
    }
    int y = _y + _height;
    string s;
    ostringstream convert;
    convert << frame_num;
    s = convert.str();
    ofSetColor(ofColor::black); // shadow
    ofDrawBitmapString(s, x + 1, y + 1);
    ofSetColor(ofColor::peachPuff);
    ofDrawBitmapString(s, x, y);
}

// TODO: Make this work
ofFbo *Timeline::getCurFbo() {
    pthread_mutex_lock(&_frame_mutex);
    
    int cur_frame = _getCurFrameNum();
    
    ofFbo *retval = _frames[cur_frame].getCurFbo();

    pthread_mutex_unlock(&_frame_mutex);
    return retval;
}

// Responsible for drawing the timeline, NOT the contents of the frames (use Timeline::drawCurFrame for that)
// We draw in the middle of the timeline the current frame
// Going into the draw function, the only info we need is _cur_frame, a pointer to the frame vector
// and a pointer to the layer vector for each frame vector (which we get from the frame).
// We start by drawing cur frame and it's layer in the middle.  We color it to indicate it is cur frame
// TODO: Draw the number line at the bottom
void Timeline::draw() {
    pthread_mutex_lock(&_click_mutex);
    pthread_mutex_lock(&_frame_mutex);
    
    // first calculate the coordinates of the middle of the timeline in x dimension, bottom of timeline in y
    int init_x = _x + _width / 2;
    int init_y = _y + _height - TIMELINE_EDGE_SPACER - TIMELINE_FRAME_SIZE;
    int cur_x = 0;
    int cur_y = 0;
    int cur_frame = _getCurFrameNum();
    
    // if we need to update the clicks list because the timeline has changed, destroy the old one
    if (_bUpdateClickList) {
        while(_clicks.size() > 0) {
            _clicks.pop_back();
        }
    }
    
    //draw background of timeline
    ofSetColor(COLOR_TIMELINE_BRACKGROUND);
    ofRect(_x, _y, _width, _height);
    
    // next draw the current frame at (mid_x, mid_y)
    cur_x = init_x;
    cur_y = init_y;
    draw_frame_num(cur_x, cur_frame);
    drawFrameRects(cur_x, cur_y, cur_frame, true);
    
    // next draw forward from the cur frame
    cur_x = init_x;
    cur_y = init_y;
    for(int f = cur_frame + 1; f < _frames.size(); f++) {
        cur_y = init_y;
        cur_x += TIMELINE_FRAME_SPACER + TIMELINE_FRAME_SIZE;
        draw_frame_num(cur_x, f);
        drawFrameRects(cur_x, cur_y, f, false);
        // make sure we're not drawing off the end of the timeline
        if(cur_x > (_x + _width - TIMELINE_EDGE_SPACER - TIMELINE_FRAME_SIZE)) {
            break;
        }
    }
    
    // next draw backward from the cur frame
    cur_x = init_x;
    cur_y = init_y;
    for(int f = cur_frame - 1; f >= 0; f--) {
        cur_y = init_y;
        cur_x -= (TIMELINE_FRAME_SPACER + TIMELINE_FRAME_SIZE);
        draw_frame_num(cur_x, f);
        drawFrameRects(cur_x, cur_y, f, false);
        // make sure we're not drawing off the end of the timeline
        if(cur_x < (_x + TIMELINE_EDGE_SPACER)) {
            break;
        }
    }
    
    _bUpdateClickList= false;
    pthread_mutex_unlock(&_frame_mutex);
    pthread_mutex_unlock(&_click_mutex);
}

void dbg_error() {
    cout << "This shouldn't have happened!" << endl;
}


// sets the current frame, either as an offset from the current frame, or
// 	as an absolute value from frame 0
void Timeline::setCurFrame(int pos, int method) {
    pthread_mutex_lock(&_frame_mutex);
    
    // first find the old cur frame and set it to no longer be the cur frame
    int old_cur = BAD_CUR_VAL;
    for(int i = 0; i < _frames.size(); i++) {
        if(true == _frames[i].getbCurFrame()) {
            old_cur = i;
            break;
        }
    }
    
    if(BAD_CUR_VAL == old_cur ) {
        cout << "couldn't find cur frame in the timeline!" << endl;
        dbg_error();
    }

    // determine the new positon and clamp it between 0 and _frame.size() -1
    int new_pos = 0;
    switch (method) {
        case RELATIVE:
            new_pos = ofClamp(old_cur + pos, 0, _frames.size()-  1);
            break;
        case ABSOLUTE:
            new_pos = ofClamp(pos, 0, _frames.size() - 1);
            break;
        default:
            cout << "Invalid method in Timeline::setbCurFrame" << endl;
            dbg_error();
    }
    
    // set the new position to have it's cur_frame variable be true
    _frames[old_cur].setbCurFrame(false);
    _frames[new_pos].setbCurFrame(true);
    _bUpdateClickList = true;
    
    pthread_mutex_unlock(&_frame_mutex);
}

// set the cur layer.  can only be used on current frame
// basically a pass through function to the _layers vector of the current frame
void Timeline::setCurLayer(int pos, int method) {
    pthread_mutex_lock(&_frame_mutex);
    int cur_frame = _getCurFrameNum();
    _frames[cur_frame].setCurLayer(pos, method);
    pthread_mutex_unlock(&_frame_mutex);
}

// get the index of the cur frame by looping through the frames and
// checking their cur_frame variables
// MUST be called with the _frame_mutex already locked!
int Timeline::_getCurFrameNum() {
    int retval = BAD_CUR_VAL;
    for(int i = 0; i < _frames.size(); i++) {
        if(true == _frames[i].getbCurFrame()) {
            retval = i;
            break;
        }
    }
    if(BAD_CUR_VAL == retval) {
        // shouldn't hit this case
        cout << "couldn't find the cur frame number!" << endl;
        dbg_error();
    }
    return retval;
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

    _bUpdateClickList = true;
    
    // TODO if we resize, should we resize all the fbo's?
    // or should we keep them small and if they try to draw outside of the original region
    // for that layer, it won't work?
    // or should we stretch the fbo's to the new dimension (almost certainly no, but it's an idea)
    return;
}


// TODO: Take into account which mousekey was pressed
void Timeline::mousePress(const int x, const int y, int mousekey) {
    // check if the click even happened in the timeline
    if ((x < _x) || (y < _y)) {
        return;
    }
    else if ((x >= _x + _width) || (y >= _y + _height)) {
        return;
    }
    
    pthread_mutex_lock(&_click_mutex);
    // iterate through the drawn objects and check if it is in the region of clickable object
    for(int i = 0; i < _clicks.size(); i++) {
        if((x >= _clicks[i].x) && (x < _clicks[i].x + _clicks[i].size)) {
            if((y >= _clicks[i].y) && (y < _clicks[i].y + _clicks[i].size)) {
                setCurFrame(_clicks[i].frame_num, ABSOLUTE);
                setCurLayer(_clicks[i].layer_num, ABSOLUTE);
                // TODO: We don't necessarily have to do this if layer changed by frame did not
                // if there is slowdown, check for this
                _bUpdateClickList = true;
                break;
            }
        }
    }
    pthread_mutex_unlock(&_click_mutex);
}


