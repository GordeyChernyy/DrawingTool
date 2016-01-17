//
//  MeshBrush.hpp
//  DrawingTool
//
//  Created by Zerc on 1/16/16.
//
//

#ifndef MeshBrush_hpp
#define MeshBrush_hpp

#include "ofMain.h"
#include "BrushBase.h"

class MeshBrush : public BrushBase {
public:
    MeshBrush(){
        setup();
    }
    ofMesh mesh;
    void clear();
    void clearHistory(); // remove all elements from history
    void cleanHistory(); // resize history
    void setup();
    void onMouseUp(int x, int y);
    void onMouseDown(int x, int y);
    void drawToCanvas(float x, float y, float pressure); // draw to fbo located in this class
    void drawBrush(float x, float y, float pressure, ofColor color); // draw lines
    
    void updateCanvas(ofFbo *fbo, float x, float y, float pressure, ofColor col); // pass fbo
    void draw(); // draw fbo located in this class
    void changeBrush(int &brushType);
    void resize();
    void begin();
    void end();
    void update();
    string name();
    ofFbo canvas;
    ofParameter<int> size;
    ofParameter<int> brushType;
    ofParameter<string> brushTypeS;
    ofParameter<int> historyMax;
    ofParameter<float> opacity;
    ofParameter<float> lineWidth;
    ofColor col;
    
    vector<ofVec2f> history;
};
#endif /* MeshBrush_hpp */
