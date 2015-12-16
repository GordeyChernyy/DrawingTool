#ifndef TIMELINE_TYPES_H
#define TIMELINE_TYPES_H
#include "ofMain.h"
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

class Layer {
    private:
        ofFbo fbo;
        int layerNum;
        int x, y;
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
    std::vector<Layer> layers;
    bool _bcurrFrame;
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
    void clear(int layerNum);
};

// test max buffers that can be allocated
/*typedef struct _test {
    unsigned char *fbo;
    struct _test *next;
} test;*/
class Clip {
private:
    int currFrame;
    int currLayer;
    int numLayers;
    int inPoint;
    int outPoint;
    vector<Frame> frames;
public:
    Clip();
    int *getCurrFrameNum (){return &currFrame;}
    int *getCurrLayerNum (){return &currLayer;}
    int *getNumLayers (){return &numLayers;}
    int *getInPoint (){return &inPoint;}
    int *getOutPoint (){return &outPoint;}
    
    Frame *getCurrFrame (){return &frames[currFrame];}
    Frame *getFrame (int num){return &frames[num];}
    ofFbo *getCurrFbo (){return frames[currFrame].getCurFbo(currLayer);}
    vector<Frame> *getFrames (){return &frames;}
    
    void setOutPointToCurrent (){outPoint = currFrame;};
    void setInPointToCurrent (){inPoint = currFrame;};
};
class Timeline {
    private:
        ofTrueTypeFont font;
        int frameRate;
        int _x, _y, width, height;
        int frameWidth, frameHeight;
 
        vector<Clip>  clips;
        int currClip;
    
        bool isPlaying;
    public:
        Timeline();
    
        bool getPlaying() { return isPlaying;}
        vector<Frame> *getFrames();
        int *getCurrFrameNum();
        Frame *getCurrFrame();
        Frame *getFrame(int num);
        ofFbo *getCurFbo();
        Clip *getCurrClip();
        int *getCurrLayerNum();
        int *getNumLayers();
        int *getInPoint();
        int *getOutPoint();
        int getFrameCount();
    
        void setup(int x, int y, int _width, int _height);
        void setFrameRate(int _frameRate);
        void setPlaying(bool newval) { isPlaying = newval; }
        void setInPointToCurrent();
        void setOutPointToCurrent();
        void setCurLayer(int pos, int method);
        void setCurFrame(int pos, int method);
    
        void addFrameNextTo();
        void addFrame();
        void addLayer();
        void beginBlend();
        void endBlend();
        void windowResize(int w, int h);
    
        void drawCurFrame();
        void drawOnionSkin(int alpha); // just before and after
        void drawTimeline();
        void drawFrameNum(int x, int frame_num);
    
        void clearCurFrame();
        void checkTimelineResize();
        void delCurFrame();
    
        int countAllocatedFbos();
};
#endif
