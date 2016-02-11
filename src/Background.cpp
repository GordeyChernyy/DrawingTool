//
//  Background.cpp
//  DrawingTool
//
//  Created by S l on 11/14/15.
//
//

#include "Background.h"
#define NOT_IN_USE -1

void BackgroundManager::draw() {
    
    int num_x_elems = 1.0 / x_elem_spacing * 1.25;
    int num_y_elems = 1.0 / y_elem_spacing;
    int num_z_elems = 1.0 / z_elem_spacing * 3.0;
    
    float x_step = ofGetWidth() * x_elem_spacing;
    float y_step = ofGetHeight() * y_elem_spacing;
    float z_step = z_distance * z_elem_spacing;
    
    float cur_x = .0;
    float cur_y = .0;
    float cur_z = .0;
    
    
    if(backgroundOn) {
        // TODO: Should this only be called at the beginining of the main draw loop in ofApp.cpp?
        // TODO: Should we query the original value before changing it?
        ofEnableDepthTest();
        ofEnableAlphaBlending();
        
        static int counter = 0;
        //if(ofRandom(4.0) > 3.0) {
            counter += 1;
        //}
        //counter =0;
        counter %= _gif.getNumFrames();
        
        
        z_offset += 10;
        z_offset %= 1000;
        z_offset -= 500;
        cout << "z_offset = " << z_offset << endl;
        
        
        for(int z = 0; z < num_z_elems; z++) {
            for(int y = 0; y < num_y_elems; y++) {
                for(int x = 0; x < num_x_elems; x++) {
                    cur_x = x * x_step + x_offset;
                    cur_y = y * y_step + y_offset;
                    cur_z = z * z_step + z_offset;
                    
                    //if((cur_z > max_z) || (cur_z < min_z)) {
                    //    continue;
                    //}
                    
                    /*shader.begin();
                    shader.setUniform1f("phase", ofGetElapsedTimef());
                    shader.setUniform1f("distortAmount", .3);
                    shader.setUniform1f("red", (float)(x * 62) / 256.0);
                    shader.setUniform1f("green", (float)(y * 63) / 256.0);
                    shader.setUniform1f("blue", (float)(z * 64) / 256.0);
                    shader.setUniform1f("alpha", 1.0);
                    */
                    ofPushMatrix();
                    ofTranslate(cur_x, cur_y, cur_z);
                    _gif.draw(0, 0, counter);
                    ofPopMatrix();
                    //shader.end();
                }
            }
        }
        
        ofDisableAlphaBlending();
        ofDisableDepthTest();
    }
}

void BackgroundManager::setup(string dir, ofParameterGroup *rootParameters) {

    parameters.setName("MagritteBG");
    parameters.add(x_offset.set("x_offset", 37.0, -500.0, 500.0));
    parameters.add(x_elem_spacing.set("x_elem_spacing", 0.31, 0.0, 1.0));
    parameters.add(y_offset.set("y_offset", 30.0, -500.0, 500.0));
    parameters.add(y_elem_spacing.set("y_elem_spacing", 0.25, 0.0, 1.0));
    parameters.add(z_offset.set("z_offset", -464, -500, 500));
    parameters.add(z_elem_spacing.set("z_elem_spacing", 0.2295, 0.0, 1.0));
    parameters.add(z_distance.set("z_distance", 2142, 0, 16000));
    parameters.add(elem_scale.set("elem_scale", 2.0, 0.0, 1.0));
    parameters.add(backgroundOn.set("backgroundOn", false));
    rootParameters->add(parameters);
    _b_fixed_ratio = true;
    _elem_height = NOT_IN_USE;
    _elem_width = NOT_IN_USE;
  
    _gif.loadGif(dir);
    _gif.setAnchorPercent(.0, .0);
    _gif.setScale(elem_scale);
    
    shader.load( "shaderVert.c", "shaderFrag.c" );
}
