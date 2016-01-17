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
    brushes.push_back(new MeshBrush());
    
    parameters.setName("BrushManager");
    parameters.add(currentBrush.set("currentBrush", 0, 0, brushes.size()-1));
    for (auto brush:brushes) parameters.add(brush->parameters);
    rootParameters->add(parameters);
}
BrushBase *BrushManager::getCurrentBrush(){
    return brushes[currentBrush];
}
BrushBase *BrushManager::getBrush(string name){
    for(auto brush: brushes){
        if(brush->name() == name){
            return brush;
        }
    }
    return;
}
void BrushManager::setCurrentBrush(int num){
    if(num<=currentBrush.getMax()){
        currentBrush = num;
    }
}