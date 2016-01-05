#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(FRAME_RATE);
    
    win.setup();
    brushM.setup();
    canvasM.setup(ofGetWidth(), ofGetHeight());
    uiM.add(canvasM.parameters);
    uiM.add(brushM.parameters);
    uiM.setup();
    
    ofBackground(uiM.bgColor);
}
//--------------------------------------------------------------
void ofApp::update(){

}
//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(uiM.bgColor);
    ofEnableAlphaBlending();
    ofSetColor(255, 255);
    // draw
    canvasM.draw();
    brush()->draw();
    ofDisableAlphaBlending();
    uiM.draw();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'x':
            canvasM.getTimeline()->autoFrameHandleSwitch();
            break;
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case '4':
            break;
        case ',':
            brushM.setCurrentBrush(0);
            break;
        case '.':
            brushM.setCurrentBrush(1);
            break;
        case '0':
            break;
        case 9:
            uiM.showGui ^= true;
            break;
        case 'q':{
            uiM.expandPrev();
            break;
        }
        case 'w':{
            uiM.expandNext();
            break;
        }
        case '+': {
            canvasM.getTimeline()->addLayer();
            break;
        }
        case '-': {
            canvasM.getTimeline()->clearCurFrame();
            break;
        }
        case 's': {
            canvasM.getTimeline()->addAndMove();
            break;
        }
        case 'd': {
            canvasM.getTimeline()->moveLeft();
            break;
        }
        case 'f': {
            canvasM.getTimeline()->moveRight();
            break;
        }
        case 'r': {
            canvasM.getTimeline()->setCurLayer(1, RELATIVE);
            break;
        }
        case 'v': {
            canvasM.getTimeline()->setCurLayer(-1, RELATIVE);
            break;
        }
        case ' ': {
            canvasM.getTimeline()->playPause();
            break;
        }
        case 'i': {
            canvasM.getTimeline()->setInPointToCurrent();
            break;
        }
        case 'o': {
            canvasM.getTimeline()->setOutPointToCurrent();
            break;
        }
        case OF_KEY_BACKSPACE: {
            canvasM.getTimeline()->delCurFrame();
            break;
        }
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    brush()->updateCanvas(fbo(), x, y, 0.3, ofColor(0));
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    brush()->onMouseDown(x, y);
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    canvasM.autoFrameHandle();
    brush()->onMouseUp(x, y);
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

BrushBase *ofApp::brush(){
    return brushM.getCurrentBrush();
}
ofFbo *ofApp::fbo(){
    return canvasM.getCurrFbo();
}
