#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableAlphaBlending();
    kaleidoscope.setup();
    brush.setup();
    movingFbo.setup();
    brushTr.setup();
    stageParam.setName("stageParam");
    stageParam.add(brushMode.set("brushMode", 0, 0, 1));
    stageParam.add(showInfo.set("showInfo", true));
    stageParam.add(enableMouse.set("enableMouse", true));
    stageParam.add(enableKaleidoscope.set("enableKaleidoscope", false));
    stageParam.add(enableMovingFbo.set("enableMovingFbo", false));
    stageParam.add(pointerSize.set("pointerSize", 5., 0., 10.));
    stageParam.add(pointerColor.set("pointerColor", ofColor(255, 255), ofColor(0, 0), ofColor(255, 255)));

    parameters.setName("parameters");
    parameters.add(stageParam);
    parameters.add(brush.parameters);
    parameters.add(brushTr.parameters);
    parameters.add(kaleidoscope.parameters);
    
    gui.setup(parameters);
    gui.loadFromFile("settings.xml");
    gui.minimizeAll();
    vector<string> names = gui.getControlNames();
    
    // set gui headers color
    for (int i = 0; i<gui.getNumControls(); i++) {
        ofColor col = ofColor(0, 162, 208);
        gui.getGroup(names[i]).setHeaderBackgroundColor(col);
        gui.getGroup(names[i]).setTextColor(ofColor(0));
        gui.getGroup(names[i]).setBorderColor(col);
    }
    
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);
    ofBackground(0);
    
    drag = false;
    currentParameter = 0;
    showGui = true;
    font.loadFont("Arial.ttf", 12);
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
    
    my_timeline.setup(ofGetWidth() / 8, ofGetWindowHeight() * .75, ofGetWindowWidth() * (6.0/8.0), ofGetWindowHeight() * .2);

}

//--------------------------------------------------------------
void ofApp::update(){
    //if (enableMovingFbo) movingFbo.update();
    // movingFbo -+
    //            +-->  kaleidoscope shader
    // canvas ----+
    
    if (enableKaleidoscope) {
        kaleidoscope.update(canvas_ptr, mouseX, mouseY);
    }
    canvas_ptr = my_timeline.getCurFbo();
}

//--------------------------------------------------------------
// TODO: Is there a race condition if we get the current fbo, then change frames?
// TODO: Fbo passthrough
void ofApp::draw(){
    //ofDisableAlphaBlending(); // TODO: blink screen when ofDrawBitmapString
    ofEnableAlphaBlending();
    ofSetColor(255, 255);
    if(enableKaleidoscope){
        kaleidoscope.draw();
    }else{
        my_timeline.drawCurFrame();
        
        switch (brushMode) { // draw any elements out of canvas
            case 0: // Dream Catcher Brush
                break;
            case 1: // Triangle Brush
                brushTr.draw(); // this will draw white triangle
                break;
            default:
                break;
        }
    }
    my_timeline.draw();
    ofDisableAlphaBlending();
    if (showGui) {
        gui.draw();
        if (showInfo) {
            string b = "";
            switch (brushMode) {
                case 0:
                    b = "Dream Catcher Brush";
                    break;
                case 1:
                    b = "Triangle Brush";
                    break;
                    
                default:
                    break;
            }
            string s =
                     "-- KEY ----------------------------------\n";
            s.append("1, 2, 3, 4    change colors\n");
            s.append("TAB           hide gui\n");
            s.append("q, w          cycle through parameters\n");
            s.append("SPACE         clear image\n");
            s.append("e             triangle brush\n");
            s.append("r             dream catcher brush\n");
            s.append("-- INFO ---------------------------------\n");
            s.append("fps           "+ ofToString(ofGetFrameRate()) +"\n");
            s.append("history size  "+ ofToString(brush.history.size()) +"\n");
            s.append("brush         "+ b +"\n");
            s.append("-- TIPS ---------------------------------\n");
            s.append("Try to enable movingFbo and Kaleidoscope.\n");
            s.append("It looks interesting if you change color\n");
            s.append("and draw at the same time.\n");
            s.append("Kaleidoscope is still in development. Po-\n");
            s.append("sition of drawing is not matching. Try \n");
            s.append("to draw more on the left top corner.\n");
            ofSetColor(0, 255); // shadow
            ofDrawBitmapString(s, gui.getWidth()+41, 21);
            ofSetColor(255, 255);
            ofDrawBitmapString(s, gui.getWidth()+40, 20);
            ofDisableAlphaBlending();
        }
    }
    ofFill();
    ofSetColor(pointerColor);
    
    ofCircle(mouseX, mouseY, pointerSize);

    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
        case 'r':
            brushMode = 0;
            break;
        case 'e':
            brushMode = 1;
            break;
        case ' ':
            // TODO: Generalize brushTr.clear() to other types of brushes
//            canvas.begin(); ofClear(0, 0); canvas.end();
//            brush.clear();
//            movingFbo.resize();
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
        case '+': {
            cout << "add layer";
            my_timeline.addLayer(ABSOLUTE);
            break;
        }
        case '=': {
            cout << "add frame";
            my_timeline.addFrame(ABSOLUTE);
            break;
        }
        case OF_KEY_LEFT: {
            // TODO: Generalize brushTr.clear() to other types of brushes
            cout << "sub 1 from cur frame";
            my_timeline.setCurFrame(-1, RELATIVE);
            break;
        }
        case OF_KEY_RIGHT: {
            // TODO: Generalize brushTr.clear() to other types of brushes
            cout << "add 1 to cur frame";
            my_timeline.setCurFrame(1, RELATIVE);
            break;
        }
        case OF_KEY_UP: {
            // TODO: Generalize brushTr.clear() to other types of brushes
            cout << "add 1 from cur layer";
            my_timeline.setCurLayer(1, RELATIVE);
            break;
        }
        case OF_KEY_DOWN: {
            // TODO: Generalize brushTr.clear() to other types of brushes
            cout << "sub 1 to cur layer";
            my_timeline.setCurLayer(-1, RELATIVE);
            break;
        }
        /*case '-': {
            cout << "center on cur frame!";
            my_timeline.centerOnCurFrame();
            break;
        }*/
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
        switch (brushMode) {
            case 0:
                brush.updateCanvas(canvas_ptr, penX, penY, p);
                break;
            case 1:
                brushTr.updateCanvas(canvas_ptr, penX, penY, brush.activeColor);
                break;
            default:
                break;
            
        }
        brushTr.setPressure(p);
    }
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(enableMouse){
        switch (brushMode) {
            case 0:
                brush.updateCanvas(canvas_ptr, x, y, 1.);
                break;
            case 1:
                brushTr.updateCanvas(canvas_ptr, x, y, brush.activeColor);
                break;
            default:
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // TODO: Should drag only be true if it is not OF_MOUSE_BUTTON_2
    
    drag = true;
    if (button == OF_MOUSE_BUTTON_1) {
        my_timeline.mousePress(x, y, button);
    }
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
    my_timeline.windowResize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
