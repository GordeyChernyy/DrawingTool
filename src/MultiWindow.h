//
//  MultiWindow.h
//  DrawingTool
//
//  Created by Zerc on 11/18/15.
//
//
#include "ofMain.h"
#include <GLFW/glfw3.h>
#include <GLUT/GLUT.h>
#pragma once
class MultiWindow {
private:
    GLFWwindow *mainWindow, *auxWindow;
    int auxWidth, auxHeight;
public:
    MultiWindow(){}
    void setup(){
        auxWidth = ofGetWindowWidth()/2;
        auxHeight = ofGetWindowHeight()/2;
        ofSetWindowTitle("Main Window");
        mainWindow = glfwGetCurrentContext();
        int xpos, ypos;
        int winw, winh;
        glfwGetWindowSize(mainWindow, &winw, &winh);
        glfwGetWindowPos(mainWindow, &xpos, &ypos);
        auxWindow = glfwCreateWindow(auxWidth, auxHeight, "Auxiliar window", NULL, mainWindow);
        glfwSetWindowPos(auxWindow, xpos+winw, ypos);
    }
    void begin(){
        glfwMakeContextCurrent(auxWindow);
        int width, height;
        glfwGetFramebufferSize(auxWindow, &width, &height);
        glViewport(0, 0, width, height);
        auxWidth = width;
        auxHeight = height;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    void end(){
        glfwSwapBuffers(auxWindow);
        glfwPollEvents();
        glfwMakeContextCurrent(mainWindow);
    }
};
