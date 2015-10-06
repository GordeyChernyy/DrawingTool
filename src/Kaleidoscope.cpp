#include "Kaleidoscope.h"
void Kaleidoscope::setup(){
    shader.load("shaders/Kaleidoscope");
    fboFilter.allocate(ofGetWidth(), ofGetHeight());
    fboFilter.begin(); ofClear(255); fboFilter.end();
    fboCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboCanvas.begin(); ofClear(255); fboCanvas.end();
    parameters.setName("Kaleidoscope");
    parameters.add(s1.set("count", 1., 0., 1.));
    parameters.add(s2.set("offsetX", 1., 0., 1.));
    parameters.add(s3.set("offsetY", 1., 0., 1.));
    parameters.add(pos.set("pos", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth()*1.5, ofGetHeight()*1.5)));
}
void Kaleidoscope::update(int x, int y){
    ofSetColor(255, 255, 255, 255);
    fboFilter.begin();
    ofClear(255);
        shader.begin();
        setUniform(x, y);
        fboCanvas.draw(0, 0);
        shader.end();
    fboFilter.end();
}
void Kaleidoscope::setUniform(int x, int y){
    shader.setUniform2f("mousePos", x, y);
    shader.setUniform2f("windowSize", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform1f("s1", s1);
    shader.setUniform1f("s2", s2);
    shader.setUniform1f("s3", s3);
}
void Kaleidoscope::draw(){
    ofSetColor(255, 255);
    fboFilter.draw(0, 0);
}
void Kaleidoscope::begin(){
    fboCanvas.begin();
    ofClear(255);
}
void Kaleidoscope::end(){
    fboCanvas.end();
}
void Kaleidoscope::clear(){
    fboCanvas.begin();
    ofClear(255);
    fboCanvas.end();
}
void Kaleidoscope::resize(){
    fboFilter.allocate(ofGetWidth(), ofGetHeight());
    fboFilter.begin(); ofClear(255); fboFilter.end();
    fboCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboCanvas.begin(); ofClear(255); fboCanvas.end();
}
