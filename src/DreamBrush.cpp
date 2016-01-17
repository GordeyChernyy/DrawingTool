//
//  DreamBrush.cpp
//  DrawingTool
//
//  Created by Zerc on 8/6/15.
//
//

#include "DreamBrush.h"
const string NAME = "DreamBrush";

void DreamBrush::setup(){
    parameters.setName("DreamBrush");
    parameters.add(historyMax.set("historyMax", 2000, 100, 10000));
    parameters.add(brushType.set("type line", 0, 0, 1));
    parameters.add(size.set("size", 300, 0, 800));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(lineWidth.set("lineWidth", 0.2, 0., 5.));
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
    brushType.addListener(this,&DreamBrush::changeBrush);
}

void DreamBrush::updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor color){
    fbo->begin();
        drawBrush(x, y, pressure, color);
    fbo->end();
}
void DreamBrush::drawBrush(float x, float y, float pressure, ofColor color){
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofVec2f m;
    m.set(x, y);
    history.push_back(m);
    float dist_points = pressure*size*1.;
    for(int i=0; i< history.size(); i++){
        ofVec2f *h = &history[i];
        float joinchance = pressure/history.size() + m.distance(*h)/dist_points;
        if (joinchance < ofRandom(0.4)) {
            ofNoFill();
            ofSetColor(color, opacity);
            ofSetLineWidth(lineWidth);
            ofDrawLine(h->x, h->y, m.x, m.y);
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
void DreamBrush::onMouseDown(int x, int y){
    clearHistory();
}
void DreamBrush::onMouseUp(int x, int y){
    clearHistory();
}
void DreamBrush::clearHistory(){
    history.clear();
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
}

string DreamBrush::name(){
    return NAME;
}