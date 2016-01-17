//
//  Mapper.hpp
//  DrawingTool
//
//  Created by Zerc on 1/16/16.
//
//

#ifndef Mapper_hpp
#define Mapper_hpp

#include "ofMain.h"
#include "OSCReceiver.hpp"
#include "BrushBase.cpp"

class Mapper{
public:
    OSCReceiver *osc;
    BrushBase *brush;
    void setup(OSCReceiver *_osc, BrushBase *_brush){
        osc = _osc;
        osc->setup(12345);
        osc->add("/mouse/position");
        brush = _brush;
    }
    void update(){
        osc->update();
        
        int x = osc->getData("/mouse/position").valuesInt[0];
        int y = osc->getData("/mouse/position").valuesInt[1];
        
        cout <<  "x= " << x  << endl;
        cout <<  "y= " << y  << endl;
        
        float min = 0;
        float max = 1000;
        
        x = ofMap(x, 0, 400, min, max);
        y = ofMap(y, 0, 400, min, max);
        
        x = ofClamp(x, min, max);
        y = ofClamp(y, min, max);
       
        if (brush->name() == "MeshBrush"){
            ofColor color(x,y,255);
//            brush->parameters.get<ofColor>("col").set(ofColor(color));
            brush->parameters.get<ofVec2f>("pointOffset").set(ofVec2f(x,y));
        }else if (brush->name() == "DreamBrush"){
            
        }
        
    }
    
};
#endif /* Mapper_hpp */
