#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "ofxTablet.h"
#include "Kaleidoscope.h"
#include "FramesFbo.h"
#include "ofxOsc.h"
#include "KinectWin.h"
#include "TriangleBrush.h"

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
    
    ofTrueTypeFont font;
    ofFbo canvas;               // fbo which will be passed to brushes
    DreamBrush brush;           // dream catcher brush
    TriangleBrush brushTr;      // triangle brush
    movingFbo movingFbo;        // moving frames
    Kaleidoscope kaleidoscope;  // shader which distort fbo passed in
    //    KinectWin kinect;

    ofxPanel gui;
    ofxGuiGroup group;
    ofParameterGroup parameters;
    ofParameterGroup stageParam;
    int currentParameter;
    bool showGui;
    bool drag;
    ofParameter<bool> kinectDebug;
    ofParameter<int> brushMode;
    ofParameter<string> brushModeLabel;
    ofParameter<bool> showInfo;
    ofParameter<bool> enableKaleidoscope;
    ofParameter<bool> enableKinect;
    ofParameter<bool> enableMovingFbo;
    ofParameter<bool> enableMouse; // mouse and tablet has a different resolution!
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;

};
