#include "timeline_types.h"

//@@@@@@@@@@@LAYER CLASS METHODS

// returns the FBO the layer encapsulates
ofFbo *Layer::getLayer() {
    return &_layer;
}

// cur_layer is a true or false value indicating whether
void Layer::setup(int width, int height, bool b_cur_layer) {
    _layer.allocate(width, height);
    _layer.begin();
        ofClear(0,0);
    _layer.end();
    _b_cur_layer = b_cur_layer;
}

// draws the contents of the FBO
void Layer::draw() {
    _layer.draw(0,0);
}

// changes the value of _cur_layer to true or false
void Layer::setbCurLayer(bool new_val) {
    _b_cur_layer = new_val;
}

// changes the value of _cur_layer to true or false
bool Layer::getbCurLayer() {
    return _b_cur_layer;
}
