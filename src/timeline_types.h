#ifndef TIMELINE_TYPES_H
#define TIMELINE_TYPES_H
#include "ofMain.h"
#include "ofxLayerMask.h"
#include <vector>
#include <pthread.h>

#define TIMELINE_FRAME_SIZE 15
#define TIMELINE_FRAME_SPACER 5
#define TIMELINE_NUMLINE_SPACER 5
#define TIMELINE_EDGE_SPACER 15
#define FRAME_NUMBER_GRANULARITY 3

#define ABSOLUTE 1
#define RELATIVE 2

#define COLOR_TIMELINE_BRACKGROUND ofColor::fromHex(0xCC9314)
#define COLOR_CURRENT_DRAWSPACE ofColor::fromHex(0xFFFFAA)
#define COLOR_CUR_FRAME ofColor::fromHex(0xD4D46A)
#define COLOR_FRAME ofColor::fromHex(0x40FFE4)

#define RESERVE_CLICKS 2000
#define RESERVE_FRAMES 2000

#define BAD_CUR_VAL -1

#define NUM_LAYERS 5
#define NUM_FRAMES 100

#define LOV_constant 0.414213562
#define QUICK_DISTANCE_THRESHOLD 100

#define FRAME_NOT_VISIBLE -1

void dbg_error();

typedef struct _click_frame_ {
    int x;
    int y;
    int size;  // not seperate width and height because square shape is assumed
//    int shape;
    int frame_num;
    int layer_num;
} ClickFrame;

class Layer {
    private:
        ofFbo _layer;
        bool _b_cur_layer;
    public:
        ofFbo *getLayer();
        void setup(int width, int height, bool _b_cur_layer);
        void draw();
        void setbCurLayer(bool new_val);
        bool getbCurLayer();
};

class Frame {
    private:
        ofxLayerMask _masker;
        std::vector <int> _mask_layers;
        std::vector<Layer> _layers;
        bool _b_cur_frame;
    public:
        std::vector<Layer> getLayers();
        int getNumLayers();
        int getCurLayer();
        ofFbo *getCurFbo();
        bool getbCurFrame();
        void setbCurFrame(bool newval);
        void setCurLayer(int pos, int method);
        void setup(int width, int height, bool cur_frame);
        void draw();
        void addLayer(int method, int width, int height);
};

class Timeline {
    private:
        int _x, _y, _width, _height;
        int _frame_width, _frame_height;
        std::vector<Frame> _frames;
        std::vector<ClickFrame> _clicks;  // test speed as a vector, if too slow, do malloc/ realloc with bigger and bigger chunks
        bool _bUpdateClickList;
        pthread_mutex_t _frame_mutex;              // used whenever iterating through the frame vector or modifying its contents
        pthread_mutex_t _click_mutex;              // used whenever the frame vector is accessed
        int _getCurFrameNum();              //_frame_mutex MUST be held when this is called

    public:
        Timeline();
        void setup(int x, int y, int width, int height);
        void windowResize(int w, int h);
        
        void addFrame(int method);
        void addLayer(int method);
        std::vector<Frame> getFrames();
        void drawCurFrame();
        int getNumFrames();
        void draw();
        void setCurFrame(int pos, int method);
        void setCurLayer(int pos, int method);
        void mousePress(const int x, const int y, int mousekey);
        bool makeNewFrame(int frame_num, int layer_num);
        void updateClickList(int x, int y, int frame_num, int layer_num);
        void destroyClickList();
        void drawFrameRects(int cur_x, int cur_y, int frame, bool b_cur_frame);
        void draw_frame_num(int x, int frame_num);
        ofFbo *getCurFbo();
};
#endif
