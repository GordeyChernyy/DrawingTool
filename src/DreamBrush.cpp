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
    parameters.add(historyMax.set("historyMax", 2000, 100, 10000));
    parameters.add(brushType.set("type line", 0, 0, 1));
    parameters.add(size.set("size", 300, 0, 800));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(lineWidth.set("lineWidth", 0.2, 0., 5.));
    parameters.add(swatch.set("swatch", 0, 0, 12));
    parameters.add(bgColor.set("bgColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(activeColor.set("activeColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col1.set("col1", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col2.set("col2", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col3.set("col3", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(bgColor); canvas.end();
    swatch.addListener(this,&DreamBrush::changeSwatch);
    brushType.addListener(this,&DreamBrush::changeBrush);
}
void DreamBrush::drawToCanvas(float x, float y, float pressure){
    canvas.begin();
        drawBrush(x, y, pressure);
    canvas.end();
}
void DreamBrush::updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col){
    fbo->begin();
        drawBrush(x, y, pressure);
    fbo->end();
}
void DreamBrush::drawBrush(float x, float y, float pressure){
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofVec2f m;
    m.set(x, y);
    history.push_back(m);
    float dist_points = pressure*size*1.;
    for(int i=0; i< history.size(); i++){
        ofVec2f h = history[i];
        float joinchance = pressure/history.size() + m.distance(h)/dist_points;
        if (joinchance < ofRandom(0.4)) {
            ofNoFill();
            ofSetColor(activeColor, opacity);
            ofSetLineWidth(lineWidth);
            ofLine(h.x, h.y, m.x, m.y);
        }
    }
    glDisable(GL_BLEND);
    if (history.size()>historyMax) {
        history.erase(history.begin());
    }
}
void DreamBrush::cleanHistory(){
    while(history.size()>historyMax) {
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
        activeColor = bgColor;
    }else{
        activeColor = colors[num];
    }
}
void DreamBrush::changeSwatch(int &swatch){
    col1 = swatches[swatch][0];
    col2 = swatches[swatch][1];
    col3 = swatches[swatch][2];
}
void DreamBrush::changeBrush(int &_brushType){
    switch (_brushType) {
        case 0:
            brushType.setName("type line");
            break;
        case 1:
            brushType.setName("type triangle");
            break;
            
        default:
            break;
    }
}
void DreamBrush::draw(){
    ofSetColor(255, 255);
    canvas.draw(0, 0);
}
void DreamBrush::resize(){
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(255); canvas.end();
}