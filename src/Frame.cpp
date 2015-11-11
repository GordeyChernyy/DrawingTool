#include "timeline_types.h"

//@@@@@@@@@@@FRAME CLASS METHODS

// returns the layers vector from the frame
std::vector<Layer> *Frame::getLayers() {
    return &_layers;
}

// first iterate through the layers look for the one with that layer_num
// if none exists, return NULL
Layer *Frame::findLayerByLayerNum(int layer_num) {
    for(int i = 0; i < _layers.size(); i++) {
        if(layer_num == _layers[i].getLayerNum()) {
            return &_layers[i];
        }
    }
    // if we iterated through the layers and got here, that layer num does not exist
    return NULL;
}

ofFbo *Frame::getCurFbo(int layer_num) {
    Layer *tmp_layer = findLayerByLayerNum(layer_num);
    
    // if that layer_num isn't present, tmp_layer is NULL
    if(NULL == tmp_layer) {
        addLayer(ofGetWindowWidth(), ofGetWindowHeight(), layer_num);
        tmp_layer = findLayerByLayerNum(layer_num);
        if(NULL == tmp_layer) {
            dbg_error("Tried adding a layer but it didn't work!");
        }
    }

    return tmp_layer->getFbo();
}

// will either insert after cur layer
void Frame::addLayer(int width, int height, int layer_num) {
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, layer_num);
    _layers.push_back(*new_layer);
}

// width and height are the size of the stuff you're drawing on the screen (so they should be window width and height)
// cur frame is whether this is the cur frame of the timeline
void Frame::setup(int width, int height) {
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, 0);
    _layers.push_back(*new_layer);
}

// draws the content of the frame (as opposed to the frame on the timeline)
void Frame::draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    for(int i = 0; i < _layers.size(); i++) {
        _layers[i].draw();
    }
    glDisable(GL_BLEND);
}

void Frame::destroyLayers() {
    for(int i = 0; i < _layers.size(); i++) {
        _layers[i].destroy();
    }
    _layers.clear();
}