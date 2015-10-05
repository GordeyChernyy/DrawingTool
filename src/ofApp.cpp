#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kaleidoscope.setup();
    brush.setup();
    framesFbo.setup();
    brushTr.setup();
//    kinect.setup();
    
    drag = false;
    stageParam.setName("stageParam");
    currentParameter = 0;
    
    parameters.setName("parameters");
    stageParam.add(brushMode.set("brushMode", 0, 0, 1));
    stageParam.add(showInfo.set("showInfo", true));
    stageParam.add(enableMouse.set("enableMouse", true));
    stageParam.add(enableKaleidoscope.set("enableKaleidoscope", false));
    stageParam.add(enableFramesFbo.set("enableFramesFbo", false));
//    stageParam.add(enableKinect.set("enableKinect", false));
//    stageParam.add(kinectDebug.set("kinectDebug", false));
    stageParam.add(pointerSize.set("pointerSize", 5., 0., 10.));
    stageParam.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    
    parameters.add(stageParam);
//    parameters.add(kinect.parameters);
    parameters.add(brush.parameters);
    parameters.add(brushTr.parameters);
    parameters.add(framesFbo.parameters);
    parameters.add(kaleidoscope.parameters);
    
    gui.setup(parameters);
    gui.loadFromFile("settings.xml");
    gui.minimizeAll();
    vector<string> names = gui.getControlNames();
    for (int i = 0; i<gui.getNumControls(); i++) { // set headers color
        ofColor col = ofColor(0, 162, 208);
        gui.getGroup(names[i]).setHeaderBackgroundColor(col);
        gui.getGroup(names[i]).setTextColor(ofColor(0));
        gui.getGroup(names[i]).setBorderColor(col);
    }
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
    ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update(){
    kaleidoscope.update(mouseX, mouseY);
    if (enableFramesFbo) framesFbo.update();
    if (enableKaleidoscope) {
        kaleidoscope.begin();
        int x = kaleidoscope.pos->x;
        int y = kaleidoscope.pos->y;
        int w = ofGetWidth();
        int h = ofGetHeight();
        if (enableFramesFbo) { // TODO: improve kaleidoscope draw
            framesFbo.getCurrentFrame().draw(x, y, -w, -h);
        }else{
            brush.canvas.draw(x, y, -w, -h);
        }
        kaleidoscope.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofDisableAlphaBlending(); // TODO: blink screen when ofDrawBitmapString
//    if(enableKaleidoscope){
//        kaleidoscope.draw();
//    }else{
//        if (enableFramesFbo) {
//            framesFbo.draw();
//        }else{
//            brush.draw();
//        }
//    }
    if (showGui) {
        gui.draw();
        ofSetColor(255, 255);
        if (showInfo) {
            string s =
            "-- KEY ----------------------------------\n";
            s.append("1, 2, 3, 4    change colors\n");
            s.append("TAB           hide gui\n");
            s.append("q, w          cycle through parameters\n");
            s.append("-- INFO ---------------------------------\n");
            s.append("fps           "+ofToString(ofGetFrameRate())+"\n");
            s.append("history size  "+ofToString(brush.history.size())+"\n");
            s.append("-- BUGS ---------------------------------\n");
            s.append("Hide info if you have blinked screen\n");
            ofDrawBitmapString(s, gui.getWidth()+40, 20);
        }
    }
//
////    if (kinectDebug) {
////        kinect.draw();
////    }
//    ofFill();
//    ofSetColor(pointerColor);
//    ofCircle(mouseX, mouseY, pointerSize);
    brushTr.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    kinect.keyPressed(key);
    switch (key) {
        case '1':
            brush.changeColor(0);
            break;
        case '2':
            brush.changeColor(1);
            break;
        case '3':
            brush.changeColor(2);
            break;
        case '4':
            brush.changeColor(3);
            break;
        case 'f':
            brushTr.addPoint(ofVec2f(mouseX, mouseY));
            brushTr.drawToCanvas(brush.activeColor);
            break;
        case ' ':
            brushTr.clear();
            brush.clear();
            framesFbo.resize();
            break;
        case 9: // TAB key
            showGui ^= true;
            break;
        case 'q':{ //left
            currentParameter--;
            if (currentParameter<0) {
                currentParameter = gui.getNumControls()-1;
            };
            vector<string> names = gui.getControlNames();
            gui.minimizeAll();
            gui.getGroup(names[currentParameter]).maximize();
            cout <<  "name " << names[currentParameter] << endl;
            break;
        }
        case 'w':{ //right
            currentParameter++;
            if (currentParameter>gui.getNumControls()-1) {
                currentParameter = 0;
            }
            vector<string> names = gui.getControlNames();
            gui.minimizeAll();
            gui.getGroup(names[currentParameter]).maximize();
            cout <<  "name " << names[currentParameter] << endl;
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
void ofApp::tabletMoved(TabletData &data) {
    
    cout <<  "data.pressure " << data.pressure << endl;
    if (drag && !enableMouse){
        float penX = data.abs_screen[0]*ofGetScreenWidth() - ofGetWindowPositionX();
        float penYinv = ofMap(data.abs_screen[1], 0, 1, 1, 0);
        float penY = penYinv*ofGetScreenHeight() - ofGetWindowPositionY();
        float p = data.pressure;
        if(enableFramesFbo){
            framesFbo.begin();
            brush.drawBrush(penX, penY, p);
            framesFbo.end();
        }else{
            brush.drawToCanvas(penX, penY, p);
        }
    }
    brushTr.setPressure(data.pressure);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    brushTr.update(x, y, brush.activeColor);
    if(enableMouse){
        if(enableFramesFbo){
            framesFbo.begin();
            brush.drawBrush(x, y, 1.);
            framesFbo.end();
        }else{
            brush.drawToCanvas(x, y, 1.);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    drag = true;
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    brushTr.clearHistory();
    drag = false;
    brush.clearHistory();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    brush.resize();
    kaleidoscope.resize();
    framesFbo.resize();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
