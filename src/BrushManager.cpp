//
//  BrushManager.cpp
//  DrawingTool
//
//  Created by Zerc on 1/2/16.
//
//

#include "BrushManager.hpp"
void BrushManager::setup(ofParameterGroup *rootParameters){
    cout <<  "brush count in manager = " << BrushBase::getBrushCount()  << endl;
    int brushCountMax = BrushBase::getBrushCount()-1;
    dreamBrush.setup();
    triangleBrush.setup();
    parameters.setName("BrushManager");
    parameters.add(currentBrush.set("currentBrush", 0, 0, brushCountMax));
    parameters.add(dreamBrush.parameters);
    parameters.add(triangleBrush.parameters);
    rootParameters->add(parameters);
}
BrushBase *BrushManager::getCurrentBrush(){
    switch (currentBrush) {
        case 0:
            return &dreamBrush;
            break;
        case 1:
            return &triangleBrush;
            break;
        default:
            break;
    }
}
void BrushManager::setCurrentBrush(int num){
    if(num<=currentBrush.getMax()){
        currentBrush = num;
    }
}