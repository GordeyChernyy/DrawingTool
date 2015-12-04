#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(FRAME_RATE);

    kaleidoscope.setup();
    dreamBrush.setup();
    triangleBrush.setup();
    textBrush.setup("Arial.ttf", "data.txt");
    my_timeline.setup(ofGetWidth() / 8, ofGetWindowHeight() * .75, ofGetWindowWidth() * (6.0/8.0), ofGetWindowHeight() * .2);
    win.setup();
    
    parameterManager.add(dreamBrush.parameters);
    parameterManager.add(triangleBrush.parameters);
    parameterManager.add(kaleidoscope.parameters);
    parameterManager.add(textBrush.parameters);
    parameterManager.setup();
    
    ofxTablet::start();
    ofAddListener(ofxTablet::tabletEvent, this, &ofApp::tabletMoved);

    drag = false;
    
    fontsize = 11;
    font.loadFont("CourierNew.ttf", fontsize);
    
    ofBackground(parameterManager.bgColor);
    cout <<  "brushes count = " << BrushBase::getBrushCount()  << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (parameterManager.enableKaleidoscope) {
        kaleidoscope.update(canvas_ptr, mouseX, mouseY);
    }
    canvas_ptr = my_timeline.getCurFbo();
}

//--------------------------------------------------------------
// TODO: Is there a race condition if we get the current fbo, then change frames?
// TODO: Fbo passthrough
void ofApp::draw(){
    ofBackground(parameterManager.bgColor);
    ofEnableAlphaBlending();
    ofSetColor(255, 255);
    if(parameterManager.enableKaleidoscope){
        kaleidoscope.draw();
    }else{
        my_timeline.drawCurFrame();
        if(parameterManager.showOnionSkin)
            my_timeline.drawOnionSkin(parameterManager.onionSkinAlpha);
        getCurrentBrush()->draw();
    }
    my_timeline.drawTimeline();

    win.begin();
    ofBackground(255, 255);
    ofPushMatrix();
    ofScale(0.5, 0.5);
    my_timeline.playFramesDetached();
    ofPopMatrix();
    win.end();
    ofDisableAlphaBlending();

    parameterManager.draw();
    
    if (parameterManager.showGui) {
        drawInfo();
    }
    // draw mouse pointer
    ofFill();
    ofSetColor(parameterManager.pointerColor);
    ofCircle(mouseX, mouseY, parameterManager.pointerSize);
}
void ofApp::drawInfo(){
    if (parameterManager.showInfo) {
        vector<string> s;
        s.push_back("-- KEY ----------------------------------");
        s.push_back("1, 2, 3, 4    change colors");
        s.push_back("TAB           hide gui");
        s.push_back("q, w          cycle through parameters");
        s.push_back("SPACE         play image");
        s.push_back("s             create new frame");
        s.push_back("f             next frame");
        s.push_back("d             previous frame");
        s.push_back("e             triangle brush");
        s.push_back(",             dream catcher brush");
        s.push_back("-             clear image");
        s.push_back("x             releaseMode: ");
        s.push_back("x             press first time: auto cre-");
        s.push_back("              ate mode on pen release");
        s.push_back("x             press second time: auto");
        s.push_back("              next frame");
        s.push_back("x             third time: return to defau");
        s.push_back("              lt.");
        s.push_back("-- INFO ---------------------------------");
        s.push_back("fps            "+ ofToString(ofGetFrameRate()) );
        s.push_back("allocated fbos "+ ofToString(my_timeline.countAllocatedFbos()) );
        s.push_back("history size   "+ ofToString(dreamBrush.history.size()) );
        s.push_back("brush          "+ getCurrentBrush()->name() );
        s.push_back("releaseMode    "+ ofToString(parameterManager.releaseMode));
        s.push_back("-- TIPS ---------------------------------");
        s.push_back("It looks interesting if you change color");
        s.push_back("and draw at the same time.");
        s.push_back("Kaleidoscope is still in development. Po-");
        s.push_back("sition of drawing is not matching. Try ");
        s.push_back("to draw more on the left top corner.");
        int width = 280;
        int border = 20;
        int x = ofGetWidth()-width-border*2;
        int y = 20;
        int lineHeight = 13;
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(255,120);
        ofRect(x-border, y-fontsize-border, width+border*2, lineHeight*(s.size()-1)+border*2);
        ofSetColor(0, 255);
        for (int i = 0; i < s.size()-1; i++) {
            font.drawString(s[i], x, y+lineHeight*i);
        }
        ofDisableAlphaBlending();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'x':
            parameterManager.releaseMode > 1 ? parameterManager.releaseMode = 0 : parameterManager.releaseMode++;
            break;
        case '1':
            dreamBrush.changeColor(0);
            break;
        case '2':
            dreamBrush.changeColor(1);
            break;
        case '3':
            dreamBrush.changeColor(2);
            break;
        case '4':
            dreamBrush.changeColor(3);
            break;
        case ',':
            parameterManager.brushMode = 0;
            break;
        case '.':
            parameterManager.brushMode = 1;
            break;
        case '0':
// TODO: Generalize triangleBrush.clear() to other types of brushes
//            canvas.begin(); ofClear(0, 0); canvas.end();
//            dreamBrush.clear();
//            movingFbo.resize();
            break;
        case 9: // TAB key
            parameterManager.showGui ^= true;
            break;
        case 'q':{ //left
            parameterManager.expandPrev();
            break;
        }
        case 'w':{ //right
            parameterManager.expandNext();
            break;
        }
        case '+': {
            cout << "add layer"<< endl;
            my_timeline.addLayer();
            break;
        }
        case '-': {
            my_timeline.clearCurFrame();
            break;
        }
        case 's': {
            my_timeline.addFrame();
            my_timeline.setCurFrame(1, RELATIVE);
            my_timeline.setStopFrame(my_timeline.getCurFrameNum());
            break;
        }
        case 'd': {
            // TODO: Generalize triangleBrush.clear() to other types of brushes
            cout << "sub 1 from cur frame"<< endl;
            my_timeline.setCurFrame(-1, RELATIVE);
            break;
        }
        case 'f': {
            // TODO: Generalize triangleBrush.clear() to other types of brushes
            cout << "add 1 to cur frame"<< endl;
            my_timeline.setCurFrame(1, RELATIVE);
            break;
        }
        case 'r': {
            // TODO: Generalize triangleBrush.clear() to other types of brushes
            cout << "add 1 from cur layer";
            my_timeline.setCurLayer(1, RELATIVE);
            break;
        }
        case 'v': {
            // TODO: Generalize triangleBrush.clear() to other types of brushes
            cout << "sub 1 to cur layer";
            my_timeline.setCurLayer(-1, RELATIVE);
            break;
        }
        case ' ': {
            if(my_timeline.getPlaying()) {
                my_timeline.setPlaying(false);
            } else {
                my_timeline.setPlaying(true);
            }
            break;
        }
        case 'i': {
            my_timeline.setStartFrame(my_timeline.getCurFrameNum());
            break;
        }
        case 'o': {
            my_timeline.setStopFrame(my_timeline.getCurFrameNum());
            break;
        }
        case OF_KEY_BACKSPACE: {
            my_timeline.delCurFrame();
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
    if (drag && !parameterManager.enableMouse){
        float penX = data.abs_screen[0]*ofGetScreenWidth() - ofGetWindowPositionX();
        float penYinv = ofMap(data.abs_screen[1], 0, 1, 1, 0);
        float penY = penYinv*ofGetScreenHeight() - ofGetWindowPositionY();
        float p = data.pressure;
        getCurrentBrush()->updateCanvas(canvas_ptr, penX, penY, p, dreamBrush.activeColor);
        triangleBrush.setPressure(p);
    }
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(parameterManager.enableMouse){
        getCurrentBrush()->updateCanvas(canvas_ptr, x, y, 1., dreamBrush.activeColor);
    }
}

BrushBase * ofApp::getCurrentBrush() {
    switch (parameterManager.brushMode) {
        case 0:
            return &dreamBrush;
            break;
        case 1:
            return &triangleBrush;
            break;
        case 2:
            return &textBrush;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    drag = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    triangleBrush.clearHistory();
    drag = false;
    dreamBrush.clearHistory();
    switch (parameterManager.releaseMode) {
        case 0:
            break;
        case 1: // auto frame add
            my_timeline.addFrame();
            my_timeline.setCurFrame(1, RELATIVE);
            my_timeline.setStopFrame(my_timeline.getCurFrameNum());
            break;
        case 2: // auto next frame
            if(my_timeline.getCurFrame() == my_timeline.getFrameCount()-1){
                my_timeline.setCurFrame(0, ABSOLUTE);
            } else {
                my_timeline.setCurFrame(1, RELATIVE);
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    dreamBrush.resize();
    kaleidoscope.resize();
    my_timeline.windowResize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
