//
//  DreamBrush.cpp
//  DrawingTool
//
//  Created by Zerc on 8/6/15.
//
//

#include "DreamBrush.h"
void DreamBrush::setup(){
    parameters.setName("DreamBrush");
    parameters.add(size.set("size", 300, 0, 800));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(lineWidth.set("lineWidth", 0.2, 0., 5.));
    parameters.add(swatch.set("swatch", 0, 0, 12));
    parameters.add(bgColor.set("bgColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(color.set("bgColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col1.set("col1", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col2.set("col2", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col3.set("col3", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(bgColor); canvas.end();
    swatch.addListener(this,&DreamBrush::changeSwatch);
    
}
void DreamBrush::update(float x, float y, float pressure){
    canvas.begin();
    ofEnableAlphaBlending();
    ofVec2f m;
    m.set(x, y);
    history.push_back(m);
    float dist_points = pressure*size*1.;
    for(int i=0; i< history.size(); i++){
        ofVec2f h = history[i];
        float joinchance = pressure/history.size() + m.distance(h)/dist_points;
        if (joinchance < ofRandom(0.4)) {
            ofNoFill();
            ofSetColor(color, opacity);
            ofSetLineWidth(lineWidth);
            ofLine(h.x, h.y, m.x, m.y);
        }
    }
    ofDisableAlphaBlending();
    canvas.end();
    if (history.size()>2000) {
        history.erase(history.begin());
    }
}
void DreamBrush::clearHistory(){
    history.clear();
}
void DreamBrush::clear(){
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(bgColor); canvas.end();
}
void DreamBrush::changeColor(int num){
    ofColor *colors = new ofColor[3];
    colors[0] = col1;
    colors[1] = col2;
    colors[2] = col3;
    if (num==3) {
        color = bgColor;
    }else{
        color = colors[num];
    }
}
void DreamBrush::changeSwatch(int &swatch){
    col1 = swatches[swatch][0];
    col2 = swatches[swatch][1];
    col3 = swatches[swatch][2];
}
void DreamBrush::draw(){
    ofSetColor(255, 255);
    canvas.draw(0, 0);
}
void DreamBrush::resize(){
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(255); canvas.end();
}