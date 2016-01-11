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
    void setup(ofParameterGroup *rootParameters);
    void setCurrentBrush(int num);
    ofParameterGroup parameters;
    BrushBase *getCurrentBrush();
private:
    vector<BrushBase*> brushes;
    ofParameter<int> currentBrush;
};

#endif /* BrushManager_hpp */
