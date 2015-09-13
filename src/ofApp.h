#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "ofxTablet.h"
#include "ShaderDistort.h"
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
    ofxPanel gui;
    ofParameterGroup parameters;
    bool showGui;
    bool drag;
    ofParameter<bool> enableShader;
    ofParameter<float> pointerSize;
    ofParameter<ofColor> pointerColor;
    ShaderDistort shader;
};
