//
//  TextBrush.h
//  DrawingTool
//
//  Created by Zerc on 11/24/15.
//
//

#pragma once
#include "ofMain.h"
#include "BrushBase.h"
#include "TextUtility.h"
#include "Font.h"

const string NAME = "Text Brush";

class TextBrush : public BrushBase {
public:
    Font font;
    vector<string> dataletters;
    vector<string> dataLines; //declare a vector of strings to store data
    TextUtility textUtility;
    int charPos;
    
    ofParameterGroup parameters;
    ofParameter<int> fontSize;
    string fontFile;
    
    void setup (string _fontFile, string pathToTextFile){
        parameters.setName("TextBrush");
        parameters.add(fontSize.set("fontSize", 20, 20, 200));
        fontSize.addListener(this, &TextBrush::changeFontSize);

        fontFile = _fontFile;
        font.setup(fontFile, fontSize);
        textUtility.setup(pathToTextFile);
    }
    void changeFontSize (int &fontSize) {
        font.setup(fontFile, fontSize);
    }
    void updateCanvas (ofFbo *fbo, float x, float y, float pressure, ofColor col){
        fbo->begin();
        
        string letter = textUtility.getNextLetter();
        int width = font.getWidth(letter);
        ofSetColor(150);
        ofRect(x, y-fontSize, width, fontSize);
        ofSetColor(col);
        font.draw(letter, x, y);
        
        fbo->end();
    }
    void draw (){
        ofSetColor(0);
    }
    
    string name (){
        return NAME;
    };
};