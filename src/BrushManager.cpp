//
//  BrushManager.cpp
//  DrawingTool
//
//  Created by Zerc on 1/2/16.
//
//

#include "BrushManager.hpp"
void BrushManager::setup(ofParameterGroup *rootParameters){
    brushes.push_back(new TriangleBrush());
    brushes.push_back(new DreamBrush());
    brushes.push_back(new PyramidBrush());
    brushes.push_back(new TextBrush());
    
    parameters.setName("BrushManager");
    parameters.add(currentBrush.set("currentBrush", 0, 0, brushes.size()-1));
    for (auto brush:brushes) parameters.add(brush->parameters);
    rootParameters->add(parameters);
}
BrushBase *BrushManager::getCurrentBrush(){
    return brushes[currentBrush];
}
void BrushManager::setCurrentBrush(int num){
    if(num<=currentBrush.getMax()){
        currentBrush = num;
    }
}