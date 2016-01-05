//
//  BrushManager.hpp
//  DrawingTool
//
//  Created by Zerc on 1/2/16.
//
//

#ifndef BrushManager_hpp
#define BrushManager_hpp

#include "ofMain.h"
#include "BrushBase.h"
#include "TextBrush.h"
#include "DreamBrush.h"
#include "TriangleBrush.h"
#include "UIManager.h"

class BrushManager{
public:
    void setup();
    void setCurrentBrush(int num);
    ofParameterGroup parameters;
    BrushBase *getCurrentBrush();
private:
    int getBrushCount();
    ofParameter<int> currentBrush;
    TriangleBrush triangleBrush;
    DreamBrush dreamBrush;
};

#endif /* BrushManager_hpp */
