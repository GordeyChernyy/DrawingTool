#include "Frame.h"

//@@@@@@@@@@@FRAME CLASS METHODS

// returns the layers vector from the frame
std::vector<Layer> *Frame::getLayers() {
    return &layers;
}

// first iterate through the layers look for the one with that layer_num
// if none exists, return NULL
Layer *Frame::findLayerByLayerNum(int layer_num) {
    for(int i = 0; i < layers.size(); i++) {
        if(layer_num == layers[i].getLayerNum()) {
            return &layers[i];
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
            //dbg_error("Tried adding a layer but it didn't work!");
            return NULL;
        }
    }

    return tmp_layer->getFbo();
}

// will either insert after cur layer
void Frame::addLayer(int width, int height, int layer_num) {
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, layer_num);
    layers.push_back(*new_layer);
}
// width and height are the size of the stuff you're drawing on the screen (so they should be window width and height)
// cur frame is whether this is the cur frame of the timeline
void Frame::setup(int width, int height) {
    Layer *new_layer = new Layer;
    new_layer->setup(width, height, 0);
    layers.push_back(*new_layer);
    alpha = 255;
}
void Frame::setAlpha(int value){
    alpha = value;
}
// draws the content of the frame (as opposed to the frame on the timeline)
void Frame::draw() {
    ofSetColor(255, alpha);
    for(int i = 0; i < layers.size(); i++) {
        layers[i].draw();
    }
}
void Frame::clear() {
    for(int i = 0; i < layers.size(); i++) {
        layers[i].clear();
    }
}
void Frame::clear(int layerNum) {
    for(int i = 0; i < layers.size(); i++) {
        if (i == layerNum) {
             layers[i].clear();
        }
    }
}
void Frame::destroyLayers() {
    for(int i = 0; i < layers.size(); i++) {
        layers[i].destroy();
    }
    layers.clear();
}