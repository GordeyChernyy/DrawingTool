//
//  KinectWin.h
//  DrawingTool
//
//  Created by Zerc on 9/23/15.
//
//
#pragma once
#include "ofMain.h"
#include "ofxKinectV2OSC.h"

class KinectWin{
public:
    ofxKinectV2OSC kinect;
    vector<ofVec2f> joints;
    bool rHandClosed, lHandClosed;
    
    Skeleton* skeleton;
    vector<Skeleton>* skeletons;
    ofTrueTypeFont smallFont, largeFont;
    
    BodyRenderer renderer;
    ofParameterGroup parameters;
    ofParameter<int> maxJointPos;
    ofParameter<bool> thumbRight;
    ofParameter<bool> spineBase;
    ofParameter<bool> spineMid;
    ofParameter<bool> neck;
    ofParameter<bool> head;
    ofParameter<bool> shoulderLeft;
    ofParameter<bool> elbowLeft;
    ofParameter<bool> wristLeft;
    ofParameter<bool> handLeft;
    ofParameter<bool> shoulderRight;
    ofParameter<bool> elbowRight;
    ofParameter<bool> wristRight;
    ofParameter<bool> handRight;
    ofParameter<bool> hipLeft;
    ofParameter<bool> kneeLeft;
    ofParameter<bool> ankleLeft;
    ofParameter<bool> footLeft;
    ofParameter<bool> hipRight;
    ofParameter<bool> kneeRight;
    ofParameter<bool> ankleRight;
    ofParameter<bool> footRight;
    ofParameter<bool> spineShoulder;
    ofParameter<bool> handTipLeft;
    ofParameter<bool> thumbLeft;
    ofParameter<bool> handTipRight;
    
    void setup(){
        parameters.setName("KinectWin");
        parameters.add(thumbRight.set("thumbRight", false));
        parameters.add(spineBase.set("spineBase", false));
        parameters.add(spineMid.set("spineMid", false));
        parameters.add(neck.set("neck", false));
        parameters.add(head.set("head", false));
        parameters.add(shoulderLeft.set("shoulderLeft", false));
        parameters.add(elbowLeft.set("elbowLeft", false));
        parameters.add(wristLeft.set("wristLeft", false));
        parameters.add(handLeft.set("handLeft", false));
        parameters.add(shoulderRight.set("shoulderRight", false));
        parameters.add(elbowRight.set("elbowRight", false));
        parameters.add(wristRight.set("wristRight", false));
        parameters.add(handRight.set("handRight", false));
        parameters.add(hipLeft.set("hipLeft", false));
        parameters.add(kneeLeft.set("kneeLeft", false));
        parameters.add(ankleLeft.set("ankleLeft", false));
        parameters.add(footLeft.set("footLeft", false));
        parameters.add(hipRight.set("hipRight", false));
        parameters.add(kneeRight.set("kneeRight", false));
        parameters.add(ankleRight.set("ankleRight", false));
        parameters.add(footRight.set("footRight", false));
        parameters.add(spineShoulder.set("spineShoulder", false));
        parameters.add(handTipLeft.set("handTipLeft", false));
        parameters.add(thumbLeft.set("thumbLeft", false));
        parameters.add(handTipRight.set("handTipRight", false));
        
        smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
        largeFont.loadFont("selena.otf", 48);
        //The Kinect here is just an OSC receiver and parser
        //It just needs a port number and font for the debug text
        kinect.setup(12345, smallFont);
        
        //Here we get a pointer to the list of skeletons it has parsed
        //from OSC
        skeletons = kinect.getSkeletons();
        
        //We could inspect the skeletons and draw them here in ofApp
        //but for now let's pass the list to a default renderer class
        renderer.setup(skeletons, largeFont);
    }
    void update(){
        //Each frame check for new Kinect OSC messages
        kinect.update();
        for(int i = 0; i < skeletons->size(); i++) {
            lHandClosed = skeletons->at(i).getLeftHand().getState();
            if(thumbRight){
                Joint thumbRight = skeletons->at(i).getThumbRight();
                ofVec2f pos(thumbRight.x(), thumbRight.y());
                joints.push_back(pos);
            }
            if(spineBase){
                Joint spineBase = skeletons->at(i).getSpineBase();
                ofVec2f pos(spineBase.x(), spineBase.y());
                joints.push_back(pos);
            }
            if(spineMid){
                Joint spineMid = skeletons->at(i).getSpineMid();
                ofVec2f pos(spineMid.x(), spineMid.y());
                joints.push_back(pos);
            }
            if(neck){
                Joint neck = skeletons->at(i).getNeck();
                ofVec2f pos(neck.x(), neck.y());
                joints.push_back(pos);
            }
            if(head){
                Joint head = skeletons->at(i).getHead();
                ofVec2f pos(head.x(), head.y());
                joints.push_back(pos);
            }
            if(shoulderLeft){
                Joint shoulderLeft = skeletons->at(i).getShoulderLeft();
                ofVec2f pos(shoulderLeft.x(), shoulderLeft.y());
                joints.push_back(pos);
            }
            if(elbowLeft){
                Joint elbowLeft = skeletons->at(i).getElbowLeft();
                ofVec2f pos(elbowLeft.x(), elbowLeft.y());
                joints.push_back(pos);
            }
            if(wristLeft){
                Joint wristLeft = skeletons->at(i).getWristLeft();
                ofVec2f pos(wristLeft.x(), wristLeft.y());
                joints.push_back(pos);
            }
            if(handLeft){
                Joint handLeft = skeletons->at(i).getHandLeft();
                ofVec2f pos(handLeft.x(), handLeft.y());
                joints.push_back(pos);
            }
            if(shoulderRight){
                Joint shoulderRight = skeletons->at(i).getShoulderRight();
                ofVec2f pos(shoulderRight.x(), shoulderRight.y());
                joints.push_back(pos);
            }
            if(elbowRight){
                Joint elbowRight = skeletons->at(i).getElbowRight();
                ofVec2f pos(elbowRight.x(), elbowRight.y());
                joints.push_back(pos);
            }
            if(wristRight){
                Joint wristRight = skeletons->at(i).getWristRight();
                ofVec2f pos(wristRight.x(), wristRight.y());
                joints.push_back(pos);
            }
            if(handRight){
                Joint handRight = skeletons->at(i).getHandRight();
                ofVec2f pos(handRight.x(), handRight.y());
                joints.push_back(pos);
            }
            if(hipLeft){
                Joint hipLeft = skeletons->at(i).getHipLeft();
                ofVec2f pos(hipLeft.x(), hipLeft.y());
                joints.push_back(pos);
            }
            if(kneeLeft){
                Joint kneeLeft = skeletons->at(i).getKneeLeft();
                ofVec2f pos(kneeLeft.x(), kneeLeft.y());
                joints.push_back(pos);
            }
            if(ankleLeft){
                Joint ankleLeft = skeletons->at(i).getAnkleLeft();
                ofVec2f pos(ankleLeft.x(), ankleLeft.y());
                joints.push_back(pos);
            }
            if(footLeft){
                Joint footLeft = skeletons->at(i).getFootLeft();
                ofVec2f pos(footLeft.x(), footLeft.y());
                joints.push_back(pos);
            }
            if(hipRight){
                Joint hipRight = skeletons->at(i).getHipRight();
                ofVec2f pos(hipRight.x(), hipRight.y());
                joints.push_back(pos);
            }
            if(kneeRight){
                Joint kneeRight = skeletons->at(i).getKneeRight();
                ofVec2f pos(kneeRight.x(), kneeRight.y());
                joints.push_back(pos);
            }
            if(ankleRight){
                Joint ankleRight = skeletons->at(i).getAnkleRight();
                ofVec2f pos(ankleRight.x(), ankleRight.y());
                joints.push_back(pos);
            }
            if(footRight){
                Joint footRight = skeletons->at(i).getFootRight();
                ofVec2f pos(footRight.x(), footRight.y());
                joints.push_back(pos);
            }
            if(spineShoulder){
                Joint spineShoulder = skeletons->at(i).getSpineShoulder();
                ofVec2f pos(spineShoulder.x(), spineShoulder.y());
                joints.push_back(pos);
            }
            if(handTipLeft){
                Joint handTipLeft = skeletons->at(i).getHandTipLeft();
                ofVec2f pos(handTipLeft.x(), handTipLeft.y());
                joints.push_back(pos);
            }
            if(thumbLeft){
                Joint thumbLeft = skeletons->at(i).getThumbLeft();
                ofVec2f pos(thumbLeft.x(), thumbLeft.y());
                joints.push_back(pos);
            }
            if(handTipRight){
                Joint handTipRight = skeletons->at(i).getHandTipRight();
                ofVec2f pos(handTipRight.x(), handTipRight.y());
                joints.push_back(pos);
            }
        }
        while(joints.size()>1000) {
            joints.erase(joints.begin());
        }
    }
    void draw(){
//        ofBackground(ofColor::darkGray);
        //Print out strings with the values from the network
        kinect.drawDebug();
        //We passed the skeleton list pointer to the renderer earlier,
        //now we tell it to draw them
        renderer.draw();
        
        //If you want to stop using the default renderer and start
        //drawing your own graphics, uncomment this for a starting point:
        /*for(int i = 0; i < skeletons->size(); i++) {
         ofSetColor(ofColor::fromHsb(ofGetFrameNum() % 255, 255, 255));
         Joint handLeft = skeletons->at(i).getHandLeft();
         ofCircle(handLeft.x(), handLeft.y(), 60);
         Joint handRight = skeletons->at(i).getHandRight();
         ofCircle(handRight.x(), handRight.y(), 60);
         }*/
        
        //Print out commands and text
        string commands = "COMMANDS\n\n";
        commands.append("d = debug\n");
        commands.append("j = joints\n");
        commands.append("b = bones\n");
        commands.append("h = hands\n");
        commands.append("r = ranges\n");
        
        ofSetColor(ofColor::white);
        smallFont.drawString(commands, 20, 40);
        largeFont.drawString("fps:\n" + ofToString(ofGetFrameRate()), 20, ofGetHeight() - 100);
    }
    void keyPressed(int key){
        if(key == 'd') kinect.toggleDebug();
        if(key == 'j') renderer.toggleJoints();
        if(key == 'b') renderer.toggleBones();
        if(key == 'h') renderer.toggleHands();
        if(key == 'r') renderer.toggleRanges();
    }
};