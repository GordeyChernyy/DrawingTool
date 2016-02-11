//
//  VideoBrush.h
//  DrawingTool
//
//  Created by S l on 10/28/15.
//
//

#include "ofMain.h"
#pragma once
#include "BrushBase.h"
#include "Timeline.h"

// By default, OfImage holds references to the same image in two places:
//      1. In RAM, which can be accessed by image.getPixels()
//      2. In VRAM (on the graphics card), which can be accessed by image.getTextureReference()
// when you change the pixel array of image, you need to call image.update() in order to apply the changes to the corresponding texture.
// image.setUseTexture( false ) discards the texture
// To use just the texture and not the RAM, use ofTexture.
/*
 ofLoadImage( texture, fileName )􏰇􏱏􏰇􏰙􏰈􏰃􏰉􏰏􏰇texture􏰇􏰑􏰁􏰈􏰍􏰇􏰃􏰊􏰇􏰅􏰍􏰃􏰋􏰂􏰇􏰨􏰙􏰂􏰔􏰇 fileName
 ofSaveImage( texture, fileName )􏰇􏱏􏰇􏰏􏰃􏰆􏰂􏰏􏰇texture􏰇􏰄􏰈􏰇􏰅􏰍􏰃􏰋􏰂􏰇􏰨􏰙􏰂􏰔􏰇 fileName
 texture.draw( x, y ) or texture.draw( x, y, w, h )􏰇􏱏􏰇􏰉􏰁􏰃􏰚􏰏􏰇texture
 texture.loadData( data, w, h, format )􏰇􏱏􏰇􏰟􏰁􏰂􏰃􏰄􏰂􏰏􏰇texture from the pixel array data, where format is GL_RGBA, GL_RGB, or GL_LUMINANCE for 4, 3, or 1 channel images respectively.
 texture.getWidth() and texture.getHeight() are used for getting texture dimensions.
 */
// texture.draw( p[0], p[1], p[2], p[3] );
//
/*ofPoint p[4];
 p[0].x = 100;  p[0].y = 100;
 p[1].x = 300;  p[1].y = 100;
 p[2].x = 300;  p[2].y = 300;
 p[3].x = 100;  p[3].y = 300;*/

/*unsigned char *getPixels()
 Also, here is the getPixelRef() method, which returns a reference to a pixel array
 of the current frame, represented by a special class ofPixels.
 Note that the ofImage class has getPixelRef()
 too and can be used in a similar way.
 
 ofPixels pixels = video.getPixelRef(); vs.
 ofPixels &pixels = video.getPixelRef();
 //pixels.setColor( x, y, color ) to change color of pixel (x,y)
 // setFromPixels() can load a ofPixels object into an ofImage.
 
 To load all the frames of a video:
 deque<ofPixels> frames;
 frames.push_front( video.getPixelsRef() );
 if ( frames.size() > N ) {
 frames.pop_back();
 }
 ofVideoPlayer.setFrame(frameNum... 0 is first, etc)
 ofVideoPlayer.getIsMovieDone()
 ofVideoPlayer.setUseTexture()
 also has ofTexture &	 ofVideoPlayer.getTextureReference
 ofVideoPlayer.tex is a ofTexture in the class... hmmm
 */
//You can wrap any image or texture on the surface using the mesh.addTexCoord( texPoint )function.
// HeretexPointisoftheofPointtype.Itisa2Dpointthat should lie in range [0, w] × [0, h], where w × h is the size of the image that you want to use as a texture.
//  Remember that you should call this function as many times as you call the mesh.addVertex() function so that all the vertices will have texture coordinates.
// So using ofMesh we can draw an arbitrary shape
// Using ofImage we can bind the image to the shape like so:
//image.bind();
//mesh.draw();
//image.unbind();
//Use image's texture for drawing
//Draw mesh
//End using image's texture
//Both ofImage and ofTexture have a bind() method.  The ofImage bind() is a passthrough to the ofTexture bind() method



/* GL_TRUE if depth testing is enabled..
state = glIsEnabled(GL_DEPTH_TEST);

or you can do:
glGetBooleanv(GL_DEPTH_TEST, &state);
*/


/*The glPushAttrib function works a lot like glPushMatrix, except that you can select the state values to put on the stack! To save all of the current rendering state, you would call
glPushAttrib(GL_ALL_ATTRIB_BITS); */

/*
Draw the current frame
draw_frame(0);
 Load the accumulation buffer with 50% of the current frame
  glAccum(GL_LOAD, 0.5);
 Draw the last 10 frames and accumulate 5% for each
  for (i = 1; i <= 10; i ++)
{
    draw_frame( -i); glAccum(GL_ACCUM, 0.05);
};
 Display the final scene
  glAccum(GL_RETURN, 1.0);
*/

/* 
Another application of the accumulation buffer is full-scene anti-aliasing. The basic strategy is to jitter the image one-half a pixel in several directions, to blur the edges of an image but not the solid areas. Accumulating as little as four of these “jittered” scenes will produce remarkably smoother images.
*/

/* See super bible Appendix A ( Performance-Tuning OpenGL for Windows ) for tips about using the most efficient methods of openGL programming */

const string PYRAMIDBRUSHNAME = "Pyramid Brush";

class PyramidBrush : public BrushBase {
public:
        PyramidBrush(){
            setup();
        }
        
        vector<ofVec2f> sortClosest(ofVec2f p, vector<ofVec2f> coords);
        void updateCanvas(Timeline *timeline, ofFbo *fbo, float x, float y, float pressure, ofColor col);
        void onMouseDown(Timeline *timeline, ofFbo *fbo, int x, int y);
        void addPoint(vector<ofVec2f> &array, ofVec2f pos);
        void onMouseUp(Timeline *timeline, ofFbo *fbo, int x, int y);
        string name();
        void draw();
        void clear();
        void clearHistory(); // remove all elements from history
        void cleanHistory(); // resize history
        void setup();
        void drawToCanvas(float x, float y, float pressure); // draw to fbo located in this class
        void drawBrush(float x, float y, float pressure); // draw lines
        
        void changeColor(int num);
        void changeSwatch(int &swatch);
        void changeBrush(int &brushType);
        void resize();
        void begin();
        void end();
    
        void setNormals(ofMesh& mesh);
    
        vector<ofVec2f> history;
        ofFbo canvas;
        ofParameter<float> size;
        ofParameter<int> brushType;
        ofParameter<string> brushTypeS;
        ofParameter<int> historyMax;
        ofParameter<float> opacity;
        ofParameter<float> lineWidth;
        ofParameter<int> swatch;
        ofParameter<ofColor> bgColor;
        ofParameter<ofColor> activeColor;
        ofParameter<ofColor> col1;
        ofParameter<ofColor> col2;
        ofParameter<ofColor> col3;
        
        void setTrianglePoints(vector<ofVec2f> sortedCoord, ofVec2f currentPoint);
    
        ofMesh mesh;
        ofTexture meshTex;
    
        ofLight light;
    
};
