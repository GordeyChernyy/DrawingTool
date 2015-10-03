#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "ofxTablet.h"
#include "Kaleidoscope.h"
#include "FramesFbo.h"
#include "ofxOsc.h"
#include "KinectWin.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void tabletMoved(TabletData &data);
    DreamBrush brush;
    FramesFbo framesFbo;
    ofxPanel gui;
//    KinectWin kinect;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    ofxGuiGroup group;
    bool showGui;
    bool drag;
    ofParameter<bool> kinectDebug;
    ofParameter<bool> enableKaleidoscope;
    ofParameter<bool> enableKinect;
    ofParameter<bool> enableFramesFbo;
    ofParameter<bool> enableMouse; // mouse and tablet has a different resolution!
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;
    Kaleidoscope kaleidoscope; 
};
