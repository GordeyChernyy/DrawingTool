#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kaleidoscope.setup();
    brush.setup();
    movingFbo.setup();
    brushTr.setup();
    korg.setup();
    light.loadImage("light.png");
    
    stageParam.setName("stageParam");
    stageParam.add(brushMode.set("brushMode", 0, 0, 1));
    stageParam.add(showInfo.set("showInfo", true));
    stageParam.add(lightPos.set("lightPos", ofVec2f(0, 0), ofVec2f(-500, -500), ofVec2f(500, 1000)));
    stageParam.add(lightSmooth.set("lightSmooth", 0.02, 0., 1.));
    stageParam.add(lightOpacity.set("lightOpacity", 255, 0, 255));
    stageParam.add(lightScale.set("lightScale", 0.02, 0., 1.));
    stageParam.add(enableLight.set("enableLight", true));
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
    showGui = true;
    canvas.allocate(ofGetWidth(), ofGetHeight());
    canvas.begin(); ofClear(0, 0); canvas.end();
    font.loadFont("Arial.ttf", 12);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // light
    lightPosX += (lightPos->x + mouseX - lightPosX)*lightSmooth;
    korg.update();
    if (korg.sliders[0] == 0) {
        enableKaleidoscope = false;
    }
    if (korg.sliders[0] == 127) {
        enableKaleidoscope = true;
    }
    if (korg.sliders[1] == 0) {
        enableMovingFbo = false;
    }
    if (korg.sliders[1] == 127) {
        enableMovingFbo = true;
    }
    if (korg.sliders[1] == 127) {
        enableMovingFbo = true;
    }
    if (korg.sliders[7] == 0) {
        brushMode = 0;
    }
    if (korg.sliders[7] == 127) {
        brushMode = 1;
    }

    // korg: change colors
    int col = ofMap(korg.sliders[6], 0, 127, 0, 3);
    brush.changeColor(col);
   
    // korg: fade to black
    float fade = ofMap(korg.knobs[6], 0, 127, 0, 50);

    canvas.begin();
    ofEnableAlphaBlending();
    ofSetColor(0, fade);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofDisableAlphaBlending();
    canvas.end();

    if (korg.buttonsRec[7]) {
        movingFbo.resize();
    }

    // korg: kaleidoscope
    mapParameter(korg.knobs[0], kaleidoscope.s4); // distort x
    mapParameter(korg.knobs[1], kaleidoscope.s5); // distort y
    mapParameter(korg.knobs[2], kaleidoscope.s6); // offset x
    mapParameter(korg.knobs[3], kaleidoscope.s7); // offset y
    // light
    mapParameter(korg.sliders[7], lightOpacity); // distort x

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
    
    ofDisableAlphaBlending();

//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);//: blink screen when ofDrawBitmapString
    ofSetColor(255, 255);
    if(enableKaleidoscope){
        kaleidoscope.draw();
    }else{
        if (enableMovingFbo) {
            movingFbo.draw();
        }else{
            canvas.draw(0, 0);               // Canvas can accept graphics from all type of static brushes
            switch (brushMode) {             // Draw any elements out of canvas on top
                case 0:                      // Dream Catcher Brush
                    break;
                case 1:                      // Triangle Brush
                    if(drag) brushTr.draw(); // This will draw white triangle
                    break;
                default:
                    break;
            }
        }
    }
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

    if (showGui) {
        gui.draw();
        if (showInfo) {
            info();
        }
    }
    ofFill();
    ofSetColor(pointerColor);
    ofCircle(mouseX, mouseY, pointerSize);
    glDisable(GL_BLEND);
    glPopAttrib();
}
void ofApp::info(){
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
    s.append("It looks interesting if you change color \n");
    s.append("and draw at the same time.\n");
    s.append("Kaleidoscope is still in development. Po-\n");
    s.append("sition of drawing is not matching. Try \n");
    s.append("to draw more on the left top corner.\n");
    s.append("To use tablet properly you need disable  \n");
    s.append("mouse. Tablet data more precise than mo- \n");
    s.append("use data.\n");
    
    ofSetColor(0, 255); // shadow
    ofDrawBitmapString(s, gui.getWidth()+41, 21);
    ofSetColor(255, 255);
    ofDrawBitmapString(s, gui.getWidth()+40, 20);
    ofDisableAlphaBlending();
    
    ofPushMatrix();
    ofTranslate(0,ofGetHeight()/2);
    korg.draw();
    ofPopMatrix();
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
void ofApp::mapParameter(float midiValue, ofParameter<float> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
void ofApp::mapParameter(int midiValue, ofParameter<int> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
void ofApp::mapParameter(bool midiValue, ofParameter<bool> &pValue){
    pValue = ofMap(midiValue, 0, 127, pValue.getMin(), pValue.getMax());
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(enableMouse){
        drag = true;
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
void ofApp::exit(){
    korg.exit();
}
