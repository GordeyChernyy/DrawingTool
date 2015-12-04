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
#include "CoordHistory.h"
#include "TransformUtility.h"

const string NAME = "Text Brush";

class TextBrush : public BrushBase {
private:
    CoordHistory coordHistory;
    TransformUtility transformUtility;
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
        coordHistory.setup(2);

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
        transformUtility.setPosition(ofVec2f (x,y));
        transformUtility.setPivot(ofVec2f (0,0));
        transformUtility.setScale(1.0);
        transformUtility.setAngle(transformUtility.getAngle(coordHistory.getPrev(), ofVec2f (x,y) ));

        fbo->begin();
        
        string letter = textUtility.getNextLetter();
        int width = font.getWidth(letter);
        
        transformUtility.begin();
        ofSetColor(col);
        font.draw(letter, 0, 0);
        transformUtility.end();
        
        fbo->end();

        coordHistory.add(ofVec2f(x,y));
    }
    void draw (){
        ofSetColor(0);
    }
    
    string name (){
        return NAME;
    };
};