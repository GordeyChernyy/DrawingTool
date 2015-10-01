#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "ofxTablet.h"
#include "ShaderDistort.h"
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
    
    void minimizeGui(bool &isMinimize);
    void tabletMoved(TabletData &data);
    DreamBrush brush;
    FramesFbo framesFbo;
    ofxPanel gui;
    KinectWin kinect;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    ofParameter<bool> isMinimize;
    ofxGuiGroup group;
    bool showGui;
    bool drag;
    ofParameter<bool> enableShader;
    ofParameter<bool> kinectDebug;
    ofParameter<bool> enableKinect;
    ofParameter<bool> enableFramesFbo;
    ofParameter<bool> enableMouse;
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;
    ShaderDistort shader;
};
