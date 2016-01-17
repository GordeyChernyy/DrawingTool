//
//  OSCReceiver.hpp
//  DrawingTool
//
//  Created by Zerc on 1/16/16.
//
//

#ifndef OSCReceiver_hpp
#define OSCReceiver_hpp

#include "ofMain.h"
#include "ofxOsc.h"
#include "Data.h"

#define NUM_MSG_STRINGS 20
#define NUM_ARGS 5

class OSCReceiver{
private:
    int port;
    ofxOscReceiver receiver;
    int dataCount;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    Data data[NUM_MSG_STRINGS];
    string messages[NUM_MSG_STRINGS];
public:
    Data getData(string _message){
        for(auto d: data){
            if(d.message == _message){
                return d;
            }
        }
        return;
    }
    void add(string message){
        data[dataCount].message = message;
        messages[dataCount] = message;
        dataCount++;
    }
    void setup(int _port){
        current_msg_string = 0;
        dataCount = 0;
        port = _port;
        cout << "listening for osc messages on port " << port << "\n";
        receiver.setup(port);
    }
    void update(){
        // hide old messages
        for(int i = 0; i < NUM_MSG_STRINGS; i++){
            if(timers[i] < ofGetElapsedTimef()){
                msg_strings[i] = "";
            }
        }
        // check for waiting messages
        while(receiver.hasWaitingMessages()){
            ofxOscMessage m;
            receiver.getNextMessage(m);
            for (int i = 0; i < dataCount; i++){
                if(m.getAddress() == messages[i]){
                    for(int j = 0; j < m.getNumArgs(); j++){
                        if(m.getArgType(j) == OFXOSC_TYPE_INT32){
                           data[i].valuesInt[j] = m.getArgAsInt32(j);
                        }
                        else if(m.getArgType(j) == OFXOSC_TYPE_FLOAT){
                           data[i].valuesFloat[j] = m.getArgAsFloat(j);
                        }
                        else if(m.getArgType(j) == OFXOSC_TYPE_STRING){
                           data[i].valuesString[j] = m.getArgAsString(j);
                        }
                        else{
                            cout <<  "unknown type of data" << endl;
                        }
                       
                    }
                } else {
//                    cout <<  "message not found" << endl;
                }
            }
        }
    }
};

#endif /* OSCReceiver_hpp */
