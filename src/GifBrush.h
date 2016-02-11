//
//  gif.h
//  DrawingTool
//
//  Created by S l on 11/11/15.
//
//

#include "ofMain.h"
#pragma once
#include "AnimatedGif.h"
#include "Timeline.h"
#include "BrushBase.h"
#define FIRST_FRAME 0

const string GIFBRUSHNAME = "GIF Brush";

class GifBrush : public BrushBase {
    public:

        void cleanHistory();
        void clearHistory();
        //void updateCanvas(ofFbo *fbo, int frame_num);
        void setup();
        void writeToFbo(ofFbo *fbo, int frame_num);
        void drawBrush(float x, float y, float pressure);
        void changeGif(int &gifNum);
        void setGifPoints(ofVec2f anchorPoint, ofVec2f currentPoint);
        void drawGif(ofVec2f *begin, ofVec2f *end, int gifFrame);
        void draw();
        int getNumFramesInGif() { return _gif.getNumFrames(); }
        void setStartPosition(float x, float y) { _start.x = x; _start.y = y; }
        void setEndPosition(float x, float y) { _end.x = x; _end.y = y; }
        void loadGif(string dir);
        void setInitialized(bool newVal) {_bInitialized = newVal; }
    
        ofParameterGroup parameters;
        ofParameter<int> size;
        ofParameter<int> brushType;
        ofParameter<string> brushTypeS;
        ofParameter<int> historyMax;
        ofParameter<float> opacity;
        ofParameter<int> frameRate;
    
        // virtual functions from BaseBrush
        void updateCanvas(Timeline *timeline, ofFbo *fbo, float x, float y, float pressure, ofColor col);
        void onMouseDown(Timeline *timeline, ofFbo *fbo, int x, int y);
        void onMouseUp(Timeline *timeline, ofFbo *fbo, int x, int y);
        string name();
    
    private:
        AnimatedGif _gif;
        float _angle;
        int _width, _height;
        vector<ofVec2f> _history;
        ofVec2f _start, _end;
        bool _bInitialized;
        ofShader shader;
};
