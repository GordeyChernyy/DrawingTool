#ifndef TIMELINE_TYPES_H
#define TIMELINE_TYPES_H
#include "ofMain.h"
#include "Frame.h"
#include <vector>
#include <pthread.h>

#define FRAME_SIZE 10
#define NUM_SPACER 4
#define EDGE_SPACER 23
#define FRAME_NUM_GRANULARITY 3

#define ABSOLUTE 1
#define RELATIVE 2

#define COLOR_TIMELINE_BRACKGROUND ofColor::fromHex(0xCC9314)
#define COLOR_CUR_DRAWSPACE ofColor::fromHex(0xe33e90)
#define COLOR_CUR_FRAME ofColor::fromHex(0xaeaeae)
#define COLOR_FRAME ofColor::fromHex(0xc7c7c7)

#define RESERVE_FRAMES 2000

#define LOV_constant 0.414213562
#define QUICK_DISTANCE_THRESHOLD 100

// TODO: Figure out a way to set this value based on evaluating hardware or user input
#define MAX_FBO_IN_MEMORY 100

void dbg_error(string err_msg);

class Timeline {
private:
    ofTrueTypeFont font;
    int frameRate;
    int _x, _y, width, height;
    int frameWidth, frameHeight;
    
    int curlayerNum;
    int numLayers;
    int inPoint, outPoint;
    int curFrame;
    vector<Frame> frames;
    
    bool isPlaying;
    ofParameter<bool> showOnionSkin;
    ofParameter<int> onionSkinAlpha;
    ofParameter<int> autoFrameHandleMode;
public:
    ofParameterGroup parameters;
    Timeline();

    bool getPlaying() { return isPlaying;}
    vector<Frame> *getFrames(){ return &frames;};
    Frame *getCurrFrame();
    Frame *getFrame(int num);
    ofFbo *getCurFbo();
    int getNumLayers();
    int getInPoint();
    int getOutPoint();
    
    void setInPoint(int newVal) {inPoint = newVal;}
    void setOutPoint(int newVal) {outPoint = newVal;}
    int getFrameCount();

    void setup(int x, int y, int _width, int _height);
    void setFrameRate(int _frameRate);
    void setPlaying(bool newval) { isPlaying = newval; }
    void setInPointToCurrent();
    void setOutPointToCurrent();
    void setCurLayer(int pos, int method);
    void setCurFrame(int pos, int method);

    void autoFrameHandleSwitch();
    void autoFrameHandle();
    void playPause();
    void addFrameNextTo();
    void addAndMove();
    void moveLeft();
    void moveRight();
    void addFrame();
    void addLayer();
    void beginBlend();
    void endBlend();
    void windowResize(int w, int h);

    void draw();
    void drawCurFrame();
    void drawOnionSkin(int alpha); // just before and after
    void drawTimeline();
    void drawFrameNum(int x, int frame_num);

    void clearCurFrame();
    void checkTimelineResize();
    void delCurFrame();
    void clearAllFrames();

    int countAllocatedFbos();
};
#endif
