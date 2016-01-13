#pragma once

#include "ofMain.h"
#include "DreamBrush.h"
#include "ofxGui.h"
#include "Kaleidoscope.h"
#include "FramesFbo.h"
#include "Timeline.h"
#include "MultiWindow.h"
#include "Crop.hpp"
#include "UIManager.h"
#include "BrushManager.hpp"
#include "CanvasManager.hpp"

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
        void drawInfo();
    
    UIManager uiM;
    CanvasManager canvasM;
    BrushManager brushM;
    ofFbo *fbo();
    BrushBase *brush();

    MultiWindow win;
private:
    
};
