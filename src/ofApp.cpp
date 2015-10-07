#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
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
    parameters.add(movingFbo.parameters);
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
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    if (enableMovingFbo) movingFbo.update();
    
    // movingFbo -+
    //            +-->  kaleidoscope shader
    // canvas ----+
    
    if (enableKaleidoscope) {
        if (enableMovingFbo) {
            kaleidoscope.update(movingFbo.getCurrentFrame(), mouseX, mouseY);
        } else {
            kaleidoscope.update(canvas, mouseX, mouseY);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDisableAlphaBlending(); // TODO: blink screen when ofDrawBitmapString
    ofSetColor(255, 255);
    if(enableKaleidoscope){
        kaleidoscope.draw();
    }else{
        if (enableMovingFbo) {
            movingFbo.draw();
        }else{
            canvas.draw(0, 0); // canvas can accept graphics from all type of static brushes
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
    }
    if (showGui) {
        gui.draw();
        ofSetColor(255, 255);
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
            s.append("-- BUGS ---------------------------------\n");
            s.append("Hide info if you have blinked screen\n");
            ofDrawBitmapString(s, gui.getWidth()+40, 20);
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
            canvas.begin(); ofClear(0, 0); canvas.end();
            brushTr.clear();
            brush.clear();
            movingFbo.resize();
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
        if(enableMovingFbo){
            int index = movingFbo.currentIndex;
            switch (brushMode) {
                case 0: // Dream Catcher Brush
                    brush.updateCanvas(movingFbo.frames[index], penX, penY, p);
                    break;
                case 1: // Triangle Brush
                    brushTr.updateCanvas(movingFbo.frames[index], penX, penY, brush.activeColor);
                    break;
                default:
                    break;
            }
        }else{
            switch (brushMode) {
                case 0:
                    brush.updateCanvas(canvas, penX, penY, p);
                    break;
                case 1:
                    brushTr.updateCanvas(canvas, penX, penY, brush.activeColor);
                    break;
                default:
                    break;
            }
        }
        brushTr.setPressure(p);
    }
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(enableMouse){
        if(enableMovingFbo){
            int index = movingFbo.currentIndex;
            switch (brushMode) {
                case 0:
                    brush.updateCanvas(movingFbo.frames[index], x, y, 1.);
                    break;
                case 1:
                    brushTr.updateCanvas(movingFbo.frames[index], x, y, brush.activeColor);
                    break;
                default:
                    break;
            }
        }else{
            switch (brushMode) {
                case 0:
                    brush.updateCanvas(canvas, x, y, 1.);
                    break;
                case 1:
                    brushTr.updateCanvas(canvas, x, y, brush.activeColor);
                    break;
                default:
                    break;
            }
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
    movingFbo.resize();
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
