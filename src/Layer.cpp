#include "timeline_types.h"

//@@@@@@@@@@@LAYER CLASS METHODS

// returns the FBO the layer encapsulates
ofFbo *Layer::getFbo() {
    return &_fbo;
}

// cur_layer is a true or false value indicating whether
void Layer::setup(int width, int height, int layer_num) {
    _fbo.allocate(width, height);
    _fbo.begin();
        ofClear(0,0);
    _fbo.end();
    _layer_num = layer_num;
}

int Layer::getLayerNum() {
    return _layer_num;
}

void Layer::setLayer(int layer_num) {
    _layer_num = layer_num;
}

// draws the contents of the FBO
void Layer::draw() {
    _fbo.draw(0,0);
}
void Layer::clear() {
    _fbo.begin();
        ofClear(0,0);
    _fbo.end();
}
void Layer::destroy() {
    // do something to fbo?  unbind?
    _fbo.unbind();

}