//#include "ofxChromaKey.h"
#include "ofMain.h"
#pragma once

    
    // The difference between textures and character arrays is textures are stored in VRAM and char arrays
    // are stored in RAM.  My computer has 2048 MB of VRAM...
    // According to tom's hardware "GPUs will run into a hybrid mode where the drivers/GPU start
    // streaming texture data from system RAM over the PCIe bus to make up for the "missing" RAM. Since
    // system RAM is 3-5X slower than GDDR5 with much higher latency, running out of "VRAM" would translate
    // into a significant FPS loss."
    // Ok so atleast we wouldn't crash.  Maybe I should do something like store the frames in RAM, then load
    // them into VRAM?
    // If it have 1 frame of 1080p at 1920x1080 and pixel width of 3 (no alpha) that's 1920*1080*3 == 6220800
    // bytes, which is 329 frames before VRAM is exhausted (atleast the 2048 on my laptop).
    // So maybe I store the frames in RAM...
    // Eh I'll implement that later if it's needed


// Each video frame will maintain a list of all the other frames in the video
// The Video object will have a resize function, which will resize all frames
// The VideoFrame object will also have a resize function, which will resize all frames
// within the in/ out marker (maybe a video specific in / out marker?)
// For all video manipulations (position, orientation, etc.) it will be the same


// TODO: Make mipmaps for each frame, so that when the frames are shrunk, we use a smaller image
// TODO: ofEnableArbTex makes it so the textures we allocate do not have to have power of 2 sized dimensions
//          I turned this on, but is it necessary/ advisable?
// TODO: I decided to have each video frame maintain a reference to the original texture of the image
//          and a reference to any chromokeyed or modified version.  Is this unnecessary duplication for most cases?
//          The frame knows whether to use the modifed texture or not because of the bUseModified flag
class VideoFrame {
public:
    void setWidth(float newVal) {width = newVal;}
    void setHeight(float newVal) {height = newVal;}
    float getWidth() {return width;}
    float getHeight() {return height;}
    void setImg(ofTexture *newVal, float newWidth, float newHeight);
    void setXYZ(int newX, int newY, int newZ) {x=newX; y=newY; z=newZ;}
    void setIterator(std::list<VideoFrame>::iterator newVal) {iterator = newVal;}
    ofTexture *getTexture() {return &tex;}
    ofTexture *getTexMod() {return &texMod;}
    void setUseModified(bool newValue) {bUseModified = newValue;}
    bool getUseModified() {return bUseModified;}
    void draw();
    void resize(float newWidth, float newHeight, int before, int after); // update the width and height variables in VideoFrame object
    void resizeTexture(float newWidth, float newHeight, int before, int after); //actually resize texture in VRAM, TODO: Implement this
    void genMipMap();   //TODO: Implement this.

    //Note that the VideoFrame does not maintain a reference to the pixels returned by getPixels
    //so the caller is responsible for freeing the resource
    ofPixels *getPixels();
    
private:
    ofTexture tex, texMod; // the original texture and the modified texture
    
    // We store the width and the height we'd like to draw the frame, even though ofTexture stores
    // this information, because if we resize the image, we'd still like the texture in VRAM to
    // remain the same size.  If we desire to resize the texture, call VideoFrame::resizeTexture()
    float width, height;
    int x, y, z;
    std::list<VideoFrame>::iterator iterator;
    bool bUseModified = false;
    ofVec2f begin, end;
};

// TODO: Add the ability to click on the color you want to key out
// TODO: Should we include width and height in here or only in the frames?
class Video {
public:
    
    void setup(string path);
    void resize(float newWidth, float newHeight);
    void scale(float scaleFactor);
    void chromakey(int frameNum);
    void loadFramelist();
    float getThreshold();
    void setThreshold(float threshold);
    void learnBgColor(ofPixelsRef pixelSource);
    void setBgColor(ofColor col);
    ofColor getBgColor();
    void drawBgColor(int x=0, int y=0, int w=10, int h=10);
    void removeBgFromAllFrames();
    void restoreBgOnAllFrames();
    void loadFramelist(ofVideoPlayer *vid);
    void setPixels(ofPixelsRef pixels);
    void setPixels(unsigned char* pixels, int w, int h);
private:
    std::list<VideoFrame> frameList;
    bool bChromakeyed = false;
    ofColor bgColor;            //color of the green screen or whatever
    float threshold;
    int highestKey;
    bool checkForHighest;
    string vidPath;

    
};

#define MAX_VIDS 10
#define MAX_FRAMES 600

typedef enum {
    VARIABLE_DIMENSION  = 0x01,
    ORIG_DIMENSION_ANCHOR_LEFT,
    ORIG_DIMENSION_ANCHOR_CENTER,
    TEMLPATE_DIMENSION
} videoManagerMode;

// TODO: I'd like to have some way of selecting a video from the current frame and
// adjusting it's x, y, z position, slant angle, size, color, etc.
// Maybe part of what the videomanager will do is have a callback when a new frame is selected,
// and it will iterate through the list of videos for the frame, and populate a drop down menu
// for the gui that let's you select which one you'd like to modify (if in modify, as opposed to draw,
// mode.

class VideoManager {
    public:
        void addVideoPath(string path);
        void setup();
        void setVideoPath();
        Video *loadCurVideo();
        ofParameterGroup parameters;
        ofParameter<int> videoSelect;
        ofParameter<int> numFrames; //number of frames to draw when applying a video
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
    void modifyThreshold(float delta) { chromaThresh += delta; }
    float getThreshold() {return chromaThresh;}
    void setThreshold(float newVal) {chromaThresh = newVal;}
    private:
        float chromaThresh;
        std::vector<Video> allVideos; // all videos on whole timeline... should I just load one master version here and recopy/ resize for each instance?
        std::vector<Video *> curVideos; // videos in current frame/ layer... TODO: Instead of maintaining list of videos per layer, should i be a list of videos per frame??
        std::vector<string> videoPaths;
        string curVideo;
        videoManagerMode drawMode;
        int init_x, init_y; //Used for mouse click and drag
        Video *curVideoObj;
};