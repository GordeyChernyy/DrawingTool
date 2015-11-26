//
//  TextUtility.h
//  DrawingTool
//
//  Created by Zerc on 11/24/15.
//
//
#pragma once
#include <fstream>
#include "ofMain.h"

class TextUtility {
private:
    vector<string> lines;
    vector<string> words;
    vector<char> letters; // with space " "
    int charPos;
    
    void fileToLines (string path){
        //declare a file stream
        ifstream fin;
        //open your text file
        fin.open (ofToDataPath(path).c_str());
        while (fin != NULL){
            string str;
            getline(fin, str);
            lines.push_back(str);
        }
    }
    void linesToWords (){
        for (int i = 0; i < lines.size()-1; i++) {
            string &line = lines[i];
            istringstream iss(line);
            while (getline(iss, line, ' ')){
                words.push_back(line);
                cout <<  "line = " << line  << endl;
            }
        }
    }
    void linesToLetters (){
        for (int i = 0; i < lines.size()-1; i++) {
            lineToLetters(lines[i], &letters);
        }
    }
    void lineToLetters (string data, vector<char> *letters){
        for (int i = 0; i < data.length(); i++) {
            char &c = data.at(i);
            letters->push_back(c);
            
        }
    }
public:
    void setup (string path){
        fileToLines(path);
        linesToLetters();
        linesToWords();
        charPos = 0;
    }
    void clearLines (){
        lines.clear();
    }
    void clearLetters (){
        letters.clear();
    }
    void clearWords (){
        words.clear();
    }
    string getNextLetter (){
        char &c = letters[charPos];
        if (charPos++ > getLetterCount()) charPos = 0;
        return ofToString(c);
    }
    int getLetterCount (){
        return letters.size()-1;
    }
};