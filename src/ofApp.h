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
#include "timeline_types.h"
#include "MultiWindow.h"
#include "BrushBase.h"
#include "TextBrush.h"
#include "ParameterManager.h"
// TODO: Make this configurable in the GUI
#define FRAME_RATE 60

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
    DreamBrush dreamBrush;           // dream catcher brush
    TriangleBrush triangleBrush;      // triangle brush
    TextBrush textBrush;
    Kaleidoscope kaleidoscope;  // shader which distort fbo passed in
    MultiWindow win;
    ParameterManager parameterManager;
    Timeline my_timeline;
    ofFbo *canvas_ptr;
    
    bool drag;
private:
    
    BrushBase * getCurrentBrush();
};
