//
//  CoordHistory.h
//  DrawingTool
//
//  Created by Zerc on 12/3/15.
//
//

#include "ofMain.h"
#pragma once

class CoordHistory{
private:
    int maxSize;
    vector<ofVec2f> history;
    float distance;
public:
    CoordHistory(){
        distance = 0;
    }
    void setup(int _maxSize){
        maxSize = _maxSize;
    }
    void add(ofVec2f pos){
        if(pos != getPrev()){ // avoid dublicate
            history.push_back(pos);
        }
        clean();
    }
    void clean(){
        if (history.size() > maxSize) {
            history.erase(history.begin());
        }
    }
    ofVec2f getPrev(){
        if(history.size()>0)
            return history[history.size()-1];
        
    }
    void addToDistance(ofVec2f pos){
        distance += getPrev().distance(pos);
        cout <<  "distance = " << distance  << endl;
    }
    float getDistance(){
        return distance;
    }
    void clearDistance(){
        distance = 0;
    }
};

