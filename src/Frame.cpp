#include "timeline_types.h"

//@@@@@@@@@@@FRAME CLASS METHODS

// returns the layers vector from the frame
std::vector<Layer> Frame::getLayers() {
    return _layers;
}

// iterates through the layers, checking their cur_layer variable
// when it find a true one, it returns the layer index
int Frame::getCurLayer() {
    for(int i = 0; i < _layers.size(); i++) {
        if(true == _layers[i].getbCurLayer()) {
            return i;
        }
    }
    
    // shouldn't hit this case
    cout << "No layer is selected as the current layer!" << endl;
    dbg_error();
    return BAD_CUR_VAL;
}

ofFbo *Frame::getCurFbo() {
    int cur_layer = getCurLayer();
    return _layers[cur_layer].getLayer();
}

// returns the size of the layers vector
int Frame::getNumLayers() {
    return _layers.size();
}

// returns true or false, for if this is the current frame in the timeline
bool Frame::getbCurFrame() {
    return _b_cur_frame;
}

// changes the value of whether this is the current frame from true to false
void Frame::setbCurFrame(bool newval) {
    _b_cur_frame = newval;
}

// will either insert at top most layer or after cur layer
void Frame::addLayer(int method, int width, int height) {
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, false);
    switch(method) {
        case RELATIVE: {
            int cur_layer = getCurLayer();
            _layers.insert(_layers.begin() + cur_layer + 1, *new_layer);
            _mask_layers.insert(_mask_layers.begin() + cur_layer + 1, _masker.newLayer());
            break; }
        case ABSOLUTE: {
            _layers.push_back(*new_layer);
            _mask_layers.push_back(_masker.newLayer());
            break;
        }
    }
}

// bottom layer is layer 0.
void Frame::setCurLayer(int pos, int method) {
    // first find which layer is currently the cur layer, and set it to false
    int old_cur = getCurLayer();
    if(BAD_CUR_VAL == old_cur ) {
        cout << "couldn't find cur layer in the frame!" << endl;
        dbg_error();
        return;
    }
    int new_pos = 0;
    switch (method) {
        case RELATIVE:
            new_pos = ofClamp(old_cur + pos, 0, _layers.size() - 1);
            break;
        case ABSOLUTE:
            new_pos = ofClamp(pos, 0, _layers.size() - 1);
            break;
        default:
            cout << "Invalid method in Timeline::setCurFrame" << endl;
            dbg_error();
            break;
    }
    _layers[old_cur].setbCurLayer(false);
    _layers[new_pos].setbCurLayer(true);
}

// width and height are the size of the stuff you're drawing on the screen (so they should be window width and height)
// cur frame is whether this is the cur frame of the timeline
void Frame::setup(int width, int height, bool b_cur_frame) {
    _b_cur_frame = b_cur_frame;
    _masker.setup(width, height);
    
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, true);
    _layers.push_back(*new_layer);
    
    _mask_layers.push_back(_masker.newLayer());
}

// draws the content of the frame (as opposed to the frame on the timeline)
void Frame::draw() {
    for(int i = 0; i < _layers.size(); i++) {
        _masker.beginLayer(_mask_layers[i]);
            ofClear(0, 255);
            _layers[i].draw();
        _masker.endLayer(_mask_layers[i]);
        
        _masker.beginMask(_mask_layers[i]);
            ofClear(0, 0, 0, 255);
            ofSetColor(ofColor::white, 255);
            _layers[i].draw();
        _masker.endMask(_mask_layers[i]);
    }
    _masker.draw();
}

