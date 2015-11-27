#ifndef TIMELINE_TYPES_H
#define TIMELINE_TYPES_H
#include "ofMain.h"
#include <vector>
#include <pthread.h>
#include "ofxFTGLFont.h"

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

class Layer {
    private:
        ofFbo _fbo;
        int _layer_num;
    public:
        ofFbo *getFbo();
        void setup(int width, int height, int layer_num);
        int getLayerNum();
        void setLayer(int layer_num);
        void draw();
        void clear();
        void destroy();
};

class Frame {
private:
    vector<ofFbo> fbos;
    std::vector <int> _mask_layers;
    std::vector<Layer> _layers;
    bool _b_cur_frame;
    int alpha;
public:
    std::vector<Layer> *getLayers();
    ofFbo *getCurFbo(int layer_num);
    void setAlpha(int value);
    void setup(int width, int height);
    void draw();
    void addLayer(int width, int height, int layer_num);
    Layer *findLayerByLayerNum(int layer_num);
    void destroyLayers();
    void clear();
};

// test max buffers that can be allocated
/*typedef struct _test {
    unsigned char *fbo;
    struct _test *next;
} test;*/

class Timeline {
    private:
        ofxFTGLFont font;
        int _cur_layer;     // count from 0
        int _num_layers;    // count from 1
        int _cur_frame;     // count from 0
        int curPlayFrame;
        int frameRate;
        int _x, _y, _width, _height;
        int _frame_width, _frame_height;
        std::vector<Frame> _frames;
    
        bool _bPlaying;
        int _start_frame, _stop_frame;
    
        // Test max buffers that can be allocated
        //test _fbo_test;
    
    public:
        Timeline();
        void setFrameRate(int _frameRate);
        void setup(int x, int y, int width, int height);
        void windowResize(int w, int h);
    
        bool getPlaying() { return _bPlaying;}
        int getCurFrame() { return _cur_frame;}
        int getFrameCount() { return _frames.size();}
    
        void setPlaying(bool newval) { _bPlaying = newval; }
        void setStartFrame(int newval) { _start_frame = newval; }
        void setStopFrame(int newval) { _stop_frame = newval; }
    
        void playFramesDetached(); // detached from main timeline using curPlayFrame
        void addFrame();
        void addLayer();
        std::vector<Frame> *getFrames();
        void beginBlend();
        void endBlend();
        void drawCurFrame();
        void drawOnionSkin(int alpha); // just before and after
        int getNumFrames();
        void drawTimeline();
        int getCurFrameNum();
        void setCurLayer(int pos, int method);
        void setCurFrame(int pos, int method);
        void clearCurFrame();
    
        void drawFrameNum(int x, int frame_num);
        ofFbo *getCurFbo();
        void checkTimelineResize();
        int countAllocatedFbos();
        void delCurFrame();
};
#endif
