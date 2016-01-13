#include "Layer.h"

//@@@@@@@@@@@LAYER CLASS METHODS

// returns the FBO the layer encapsulates
ofFbo *Layer::getFbo() {
    return &fbo;
}

// cur_layer is a true or false value indicating whether
void Layer::setup(int width, int height, int layer_num) {
    x = 0;
    y = 0;
    fbo.allocate(width, height);
    fbo.begin();
        ofClear(0,0);
    fbo.end();
    layerNum = layer_num;
}

int Layer::getLayerNum() {
    return layerNum;
}

void Layer::setLayer(int layer_num) {
    layerNum = layer_num;
}

// draws the contents of the FBO
void Layer::draw() {
    fbo.draw(x,y);
}
void Layer::clear() {
    fbo.begin();
        ofClear(0,0);
    fbo.end();
}
void Layer::destroy() {
    // do something to fbo?  unbind?
    fbo.unbind();

}