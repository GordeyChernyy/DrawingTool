#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shader.setup();
    brush.setup();
    framesFbo.setup();
    drag = false;
    parameters.setName("parameters");
    parameters.add(isMinimize.set("isMinimize", true));
    parameters.add(enableShader.set("enableShader", false));
    parameters.add(enableFramesFbo.set("enableFramesFbo", false));
    parameters.add(pointerSize.set("pointerSize", 5., 0., 10.));
    parameters.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(brush.parameters);
    parameters.add(framesFbo.parameters);
    parameters.add(shader.parameters);
    isMinimize.addListener(this, &ofApp::minimizeGui);
    gui.setup(parameters);
    gui.loadFromFile("settings.xml");
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
}

//--------------------------------------------------------------
void ofApp::update(){
    shader.update(mouseX, mouseY);
    if (enableFramesFbo) framesFbo.update();
    if (enableShader) {
        shader.begin();
        if (enableFramesFbo) {
            framesFbo.getCurrentFrame().draw(shader.pos->x, shader.pos->y, -ofGetWidth(), ofGetHeight());
        }else{
            brush.canvas.draw(shader.pos->x, shader.pos->y, -ofGetWidth(), ofGetHeight());
        }
        shader.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(enableShader){
        shader.draw();
    }else{
        if (enableFramesFbo) {
            framesFbo.draw();
        }else{
            brush.draw();
        }
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
            framesFbo.resize();
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
        if(enableFramesFbo){
            framesFbo.begin();
            brush.drawBrush(penX, penY, p);
            framesFbo.end();
        }else{
            brush.update(penX, penY, p);
        }
    }
}
void ofApp::minimizeGui(bool &isMinimize){
    gui.minimizeAll();
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
    framesFbo.resize();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
