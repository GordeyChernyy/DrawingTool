#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shader.setup();
    brush.setup();
    drag = false;
    parameters.setName("parameters");
    parameters.add(enableShader.set(false));
    parameters.add(pointerSize.set("pointerSize", 5., 0., 10.));
    parameters.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(brush.parameters);
    parameters.add(shader.parameters);
    gui.setup(parameters);
//        CGDisplayShowCursor(kCGDirectMainDisplay);
    
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
}

//--------------------------------------------------------------
void ofApp::update(){
    shader.update(mouseX, mouseY);

}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofHideCursor();
//    ofBackground(brush.bgColor);
    if(enableShader){
        shader.begin();
        brush.canvas.draw(shader.pos->x, shader.pos->y, -ofGetWidth(), ofGetHeight());
        shader.end();
        shader.draw();
    }else{
        brush.draw();
    }
    if(showGui){
        CGDisplayShowCursor(NULL);
        gui.draw();
    }else{
        CGDisplayHideCursor(NULL);
    }
    ofFill();
    ofSetColor(pointerColor);
    ofCircle(mouseX, mouseY, pointerSize);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1': // TAB key
            brush.changeColor(0);
            break;
        case '2': // TAB key
            brush.changeColor(1);
            break;
        case '3': // TAB key
            brush.changeColor(2);
            break;
        case '4': // TAB key
            brush.changeColor(3);
            break;
        case ' ': // TAB key
            brush.clear();
            break;
        case 9: // TAB key
            showGui ^= true;
            break;

        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}
void ofApp::tabletMoved(TabletData &data) {
    cout <<  "data.pressure " << data.pressure << endl;
    if (drag){
        float penX = data.abs_screen[0]*ofGetScreenWidth() - ofGetWindowPositionX();
        float penYinv = ofMap(data.abs_screen[1], 0, 1, 1, 0);
        float penY = penYinv*ofGetScreenHeight() - ofGetWindowPositionY();
        float p = data.pressure;
        brush.update(penX, penY, p);
    }
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    drag = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    drag = false;
    brush.clearHistory();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    brush.resize();
    shader.resize();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
