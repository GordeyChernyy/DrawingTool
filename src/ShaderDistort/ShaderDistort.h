//
//  ShaderDistort.h
//  ShaderWaterColor
//
//  Created by Zerc on 7/8/15.
//
//
#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"

class ShaderDistort{
private:

    
    void setUniform(int x, int y);
public:
    ofFbo fboCanvas, fboFilter;
    ofxAutoReloadedShader shader;
    void setup();
    void update(int x, int y);
    void draw();
    void begin();
    void end();
    void clear();
    void resize();
    ofParameterGroup parameters;
    ofParameter<float> s1, s2, s3;
    ofParameter<ofVec2f> pos;
};
