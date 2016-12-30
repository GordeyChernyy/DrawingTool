#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(FRAME_RATE);
    
    ofDirectory dir;
//
//    // load fbos and settings
//    for(int i = 0; i < dir.size(); i++){
//        string name = dir.getFile(i).getBaseName();
//            ofImage img;
//            img.load(dir.getPath(i));
//            player.push_back(img);
//    }
//    
    colorM.setup(uiM.getParameters());
    brushM.setup(uiM.getParameters());
    canvasM.setup(ofGetWidth(), ofGetHeight(), uiM.getParameters());
    uiM.setup();
    
    mapper.setup(&osc, brush());
    
    ofBackground(uiM.bgColor);
    clip.load("clips/test");
//    player.load("growClip.mp4");
}
//--------------------------------------------------------------
void ofApp::update(){
    brush()->update();
//    mapper.update();
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(uiM.bgColor);
    ofEnableAlphaBlending();
    ofSetColor(255,255);
    canvasM.draw();
    brush()->draw();
    ofDisableAlphaBlending();
    uiM.draw();
//    player[0].draw(0, 0);
//    clip.draw();
//    player.draw(0, 0);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    keyIsDown[key] = true;
    if(keyIsDown[OF_KEY_COMMAND] && keyIsDown['s'] ){
        keyIsDown['s'] = false;
        clip.saveFbos(canvasM.getTimeline());
    }
    if(keyIsDown[OF_KEY_COMMAND] && keyIsDown['n'] ){
        canvasM.getTimeline()->clearTimeline();
    }
    switch (key) {
        case 'x':
            canvasM.getTimeline()->autoFrameHandleSwitch();
            break;
        case '1':
            colorM.setColorNum(0);
            break;
        case 'b': {
            clip.saveFbos(canvasM.getTimeline());
            break;
        }
        case '2':
            colorM.setColorNum(1);
            break;
        case '3':
            colorM.setColorNum(2);
            break;
        case '4':
            colorM.setColorNum(3);
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
    keyIsDown[key] = false;

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    brush()->updateCanvas(fbo(), x, y, 0.3, colorM.getCurColor());
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
    canvasM.getTimeline()->loadBg(dragInfo.files[0]);
}

BrushBase *ofApp::brush(){
    return brushM.getCurrentBrush();
}
ofFbo *ofApp::fbo(){
    return canvasM.getCurrFbo();
}
