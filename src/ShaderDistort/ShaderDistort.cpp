//
//  ShaderDistort.cpp
//  ShaderWaterColor
//
//  Created by Zerc on 7/8/15.
//
//

#include "ShaderDistort.h"
void ShaderDistort::setup(){
    shader.load("shader/shaderDist");
    shader.setMillisBetweenFileCheck(100);
    fboFilter.allocate(ofGetWidth(), ofGetHeight());
    fboFilter.begin(); ofClear(255); fboFilter.end();
    fboCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboCanvas.begin(); ofClear(255); fboCanvas.end();
    parameters.setName("ShaderDistort");
    parameters.add(s1.set("count", 1., 0., 1.));
    parameters.add(s2.set("offsetX", 1., 0., 1.));
    parameters.add(s3.set("offsetY", 1., 0., 1.));
    parameters.add(pos.set("pos", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth()*1.5, ofGetHeight()*1.5)));
}
void ShaderDistort::update(int x, int y){
    ofSetColor(255, 255, 255, 255);
    fboFilter.begin();
    ofClear(255);
        shader.begin();
        setUniform(x, y);
        fboCanvas.draw(0, 0);
        shader.end();
    fboFilter.end();
}
void ShaderDistort::setUniform(int x, int y){
    shader.setUniform2f("mousePos", x, y);
    shader.setUniform2f("windowSize", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform1f("s1", s1);
    shader.setUniform1f("s2", s2);
    shader.setUniform1f("s3", s3);
}
void ShaderDistort::draw(){
    ofSetColor(255, 255);
    fboFilter.draw(0, 0);
}
void ShaderDistort::begin(){
    fboCanvas.begin();
    ofClear(255);
}
void ShaderDistort::end(){
    fboCanvas.end();
}
void ShaderDistort::clear(){
    fboCanvas.begin();
    ofClear(255);
    fboCanvas.end();
}
void ShaderDistort::resize(){
    fboFilter.allocate(ofGetWidth(), ofGetHeight());
    fboFilter.begin(); ofClear(255); fboFilter.end();
    fboCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboCanvas.begin(); ofClear(255); fboCanvas.end();
}

