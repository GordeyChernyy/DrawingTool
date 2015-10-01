#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shader.setup();
    brush.setup();
    framesFbo.setup();
    kinect.setup();
    
    drag = false;
    parameters.setName("parameters");
    stageParam.setName("stageParam");
    currentParameter = 0;
    stageParam.add(isMinimize.set("isMinimize", true));
    stageParam.add(enableMouse.set("enableMouse", true));
    stageParam.add(enableShader.set("enableShader", false));
    stageParam.add(enableFramesFbo.set("enableFramesFbo", false));
    stageParam.add(enableKinect.set("enableKinect", false));
    stageParam.add(kinectDebug.set("kinectDebug", false));
    stageParam.add(pointerSize.set("pointerSize", 5., 0., 10.));
    stageParam.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));
    
    parameters.add(stageParam);
    parameters.add(kinect.parameters);
    parameters.add(brush.parameters);
    parameters.add(framesFbo.parameters);
    parameters.add(shader.parameters);
    isMinimize.addListener(this, &ofApp::minimizeGui);

    gui.setup(parameters);
    gui.getGroup(brush.parameters.getName()).setHeaderBackgroundColor(ofColor(255, 0, 0));
    gui.loadFromFile("settings.xml");
    // set headers color
    vector<string> names = gui.getControlNames();
    for (int i = 0; i<gui.getNumControls(); i++) {
        ofColor col = ofColor(0, 162, 208);
        gui.getGroup(names[i]).setHeaderBackgroundColor(col);
        gui.getGroup(names[i]).setTextColor(ofColor(0));
        gui.getGroup(names[i]).setBorderColor(col);
    }
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    shader.update(mouseX, mouseY);
    if (enableKinect) {
        for (int i = 0; i < kinect.joints.size(); i++) {
            if(kinect.lHandClosed) brush.history.push_back(kinect.joints[i]);
//            cout <<  "kinect.jointPos[i]= " << kinect.joints[i]  << endl;
        }
        if(kinect.lHandClosed) {
            brush.changeColor(0);
        }else{
            brush.changeColor(1);
        }
        brush.cleanHistory();
        if (kinect.joints.size()>1 ) {
            if(enableFramesFbo){
                framesFbo.begin();
                brush.drawBrush(kinect.joints[kinect.joints.size()].x,
                             kinect.joints[kinect.joints.size()].y, 1.0);
                framesFbo.end();
            }else{
                brush.update(kinect.joints[kinect.joints.size()].x,
                             kinect.joints[kinect.joints.size()].y, 1.0);
            }
            
        }
    }
    if (enableFramesFbo) framesFbo.update();
    if (enableShader) {
        shader.begin();
        int x = shader.pos->x;
        int y = shader.pos->y;
        int w = ofGetWidth();
        int h = ofGetHeight();
        if (enableFramesFbo) {
            framesFbo.getCurrentFrame().draw(x, y, -w, -h);
        }else{
            brush.canvas.draw(x, y, -w, -h);
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

        gui.draw();

    if (kinectDebug) {
        kinect.draw();
    }
    ofFill();
    ofSetColor(pointerColor);
    ofCircle(mouseX, mouseY, pointerSize);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    cout <<  "key= " << key << endl;
    kinect.keyPressed(key);
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
        case ' ':
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
            brush.update(penX, penY, p);
        }
    }
}
void ofApp::minimizeGui(bool &isMinimize){
    gui.minimizeAll();
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(enableMouse){
        if(enableFramesFbo){
            framesFbo.begin();
            brush.drawBrush(x, y, 1.);
            framesFbo.end();
        }else{
            brush.update(x, y, 1.);
        }
    }
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
