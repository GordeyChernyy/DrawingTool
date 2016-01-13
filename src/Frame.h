//
//  Frame.h
//  DrawingTool
//
//  Created by Zerc on 1/12/16.
//
//

#ifndef Frame_h
#define Frame_h
#include "Layer.h"
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

#endif /* Frame_h */
