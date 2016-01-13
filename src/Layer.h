//
//  Layer.h
//  DrawingTool
//
//  Created by Zerc on 1/12/16.
//
//

#ifndef Layer_h
#define Layer_h
#include "ofMain.h"

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
#endif /* Layer_h */
