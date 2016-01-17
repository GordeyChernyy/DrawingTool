//
//  MeshBrush.cpp
//  DrawingTool
//
//  Created by Zerc on 1/16/16.
//
//

#include "MeshBrush.hpp"

const string NAME = "MeshBrush";

void MeshBrush::setup(){
    parameters.setName("MeshBrush");
    parameters.add(historyMax.set("historyMax", 1000, 100, 10000));
    parameters.add(brushType.set("type line", 0, 0, 1));
    parameters.add(size.set("size", 300, 0, 800));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(lineWidth.set("lineWidth", 0.2, 0., 5.));
    parameters.add(pointOffset.set("pointOffset", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(1000,1000)));
    parameters.add(col.set("col", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
    brushType.addListener(this,&MeshBrush::changeBrush);
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
}
void MeshBrush::update(){

}
void MeshBrush::updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor color){
    fbo->begin();
    drawBrush(x, y, pressure, color);
    fbo->end();
}
void MeshBrush::drawBrush(float x, float y, float pressure, ofColor color){
//    col = color;
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofVec2f m;
    m.set(x, y);
    history.push_back(m);
    float dist_points = pressure*size*1.;
    int index = 0;
    ofMesh mesh2;
    mesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for(int i=0; i< history.size(); i++){
        ofVec2f *h = &history[i];
        float joinchance = pressure/history.size() + m.distance(*h)/dist_points;
        if (joinchance < ofRandom(0.9)) {
            mesh2.addVertex(ofVec3f(h->x, h->y,0));
            mesh2.addVertex(ofVec3f(m.x, m.y,0));
            mesh2.addVertex(ofVec3f(pointOffset->x, pointOffset->y,0));
            mesh2.addColor(ofColor(col,opacity));
            mesh2.addColor(ofColor(color.r-100,color.g-100, color.b-10,opacity));
            mesh2.smoothNormals(360);
        }
    }
    mesh2.draw();
    glDisable(GL_BLEND);
    if (history.size()>historyMax) {
        history.erase(history.begin());
        
    }
    
    
    
}
void MeshBrush::cleanHistory(){
    while(history.size()>historyMax) {
        history.erase(history.begin());
    }
}
void MeshBrush::onMouseDown(int x, int y){
    
    clearHistory();
}
void MeshBrush::onMouseUp(int x, int y){
    mesh.addColor(ofColor(col, opacity));
    clearHistory();
}
void MeshBrush::clearHistory(){
    history.clear();
}

void MeshBrush::changeBrush(int &_brushType){
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
void MeshBrush::draw(){
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    mesh.draw();
    glDisable(GL_BLEND);
}

string MeshBrush::name(){
    return NAME;
}