//
//  Background.h
//  DrawingTool
//
//  Created by S l on 11/14/15.
//

#include "ofMain.h"
#pragma once
#include "AnimatedGif.h"

// TODO: Make it so this fits on the timeline.  Make it so it draws from min_z to max_z for a given layer
//          on the timeline

// TODO: Generalize this from AnimatedGifs to 3d objects as a possibility for the elements as well
// TODO: Generalize this from AnimateGifs to Video objects
class BackgroundManager{
    public:
        void draw();
        void setup(string dir, ofParameterGroup *rootParameters);
        int getNumLayers() {return _num_layers;}
        void setNumLayers(int newval) {_num_layers = newval;}
        int getElemWidth() {return _elem_width;}
        void setElemWidth(int newval) {_elem_width = newval;}
        int getElemHeight() {return _elem_height;}
        void setElemHeight(int newval) {_elem_height = newval;}
        bool getFixedRatio() {return _b_fixed_ratio;}
    void setFixedRatio(bool newval) {_b_fixed_ratio = newval;}

    
    ofParameterGroup parameters;
    ofParameter<float> x_offset;
    ofParameter<float> y_offset;
    ofParameter<int> z_offset;      //TODO Fix the offsets for they're all floats or they're all ints
    ofParameter<float> x_elem_spacing; //0.0 min, 1.0 max, a percent
    ofParameter<float> y_elem_spacing; //0.0 min, 1.0 max, a percent
    ofParameter<float> z_elem_spacing; //0.0 min, 1.0 max, a percent
    ofParameter<int> z_distance;
    ofParameter<float> elem_scale;
    ofParameter<bool> backgroundOn;
    
    private:
        AnimatedGif _gif;
        int _num_layers; // number of layers in the z dimension to draw the object
        // dimensions of the elements we are drawing
        int _elem_width, _elem_height;
        bool _b_fixed_ratio; //true means use _elem_scale for drawing, false means use _elem_width and _elem_height

        ofShader shader;
};