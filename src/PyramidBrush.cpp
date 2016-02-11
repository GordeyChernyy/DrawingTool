//
//  PyramidBrush.cpp
//  DrawingTool
//
//  Created by S l on 10/28/15.
//
//

#include "PyramidBrush.h"

vector<ofVec2f> PyramidBrush::sortClosest(ofVec2f p, vector<ofVec2f> coords){
    vector<ofVec2f> pos;
    float maxDist = ofGetWidth();
    int closestIndex = 0;
    for (int i = 0; i < coords.size(); i++) {
        float dist = p.distance(coords[i]);
        if (dist < maxDist){
            maxDist = dist;
            pos.push_back(coords[i]);
        }
    }
    return pos;
}

void PyramidBrush::setTrianglePoints(vector<ofVec2f> sortedCoord, ofVec2f currentPoint){
    return;
    /*
    // create random acces to points in sorted array
    // [ . . . . . . ]
    //       |   | |
    //      min    max
    if(sortedCoord.size()==1){   // avoid zero bug
        sortedCoord.push_back(sortedCoord[0]);
    }
    int range = indexRange;
    int min = sortedCoord.size()-range;  // random index min
    int max = sortedCoord.size()-1;  // random index max
    if(sortedCoord.size() < range){  // avoid negative value
        min = 0;
    }
    int index1 = (int)ofRandom(min, max);
    int index2 = (int)ofRandom(min, max);
    trianglePoints[0] = currentPoint;
    trianglePoints[1] = sortedCoord[index1];
    trianglePoints[2] = sortedCoord[index2];
    */
}

void PyramidBrush::addPoint(vector<ofVec2f> &array, ofVec2f pos){
    array.push_back(pos);
}

void PyramidBrush::updateCanvas(Timeline *timeline, ofFbo *fbo, float x, float y, float pressure, ofColor col) {
    fbo->begin();
        ofEnableDepthTest();
    
        ofPushMatrix();
            ofTranslate(x , y, 0);
            ofScale(size, size, size);
            float time = ofGetElapsedTimef();
            float angle = time * 90;
            ofRotate(angle, 0, 1, 1);
            ofSetColor( 255, 255, 255 );  //Set white color

            meshTex.bind();
                mesh.draw();// (300,500);
            meshTex.unbind();
        ofPopMatrix();
    fbo->end();
    ofDisableDepthTest();
    return;
    
    //Use image's texture for drawing
    //Draw mesh
    //End using image's texture
/*    ofVec2f currentPoint(x,y);
    if (history.size() < 4) addPoint(history, currentPoint);
    if (history.size() > 3){
        vector<ofVec2f> points;
        points = sortClosest(currentPoint, history);   // sort array: last point in array is closest
        setTrianglePoints(points, currentPoint);
        int index = points.size()-1;
        ofVec2f a = points[index];
        ofVec2f b = currentPoint;
        float dist = dotDistance*pressure;
        if(isDistanceBigger(a, b, dist)){
            addPoint(history, currentPoint);
            drawToCanvas(fbo, col);
        }
    }
    */
}
void PyramidBrush::onMouseDown(Timeline *timeline, ofFbo *fbo, int x, int y) {
    return;
}
void PyramidBrush::onMouseUp(Timeline *timeline, ofFbo *fbo, int x, int y) {
    return;
}
void PyramidBrush::draw() {
    return;
}
string PyramidBrush::name() {
    return PYRAMIDBRUSHNAME;
}


void PyramidBrush::setNormals(ofMesh& mesh) {
    
    int nV = mesh.getNumVertices();
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm(nV);
    for(int t=0; t < nT; t++) {
        int i1 = mesh.getIndex(3*t);
        int i2 = mesh.getIndex(3*t + 1);
        int i3 = mesh.getIndex(3*t + 2);
        
        const ofPoint &v1 = mesh.getVertex(i1);
        const ofPoint &v2 = mesh.getVertex(i2);
        const ofPoint &v3 = mesh.getVertex(i3);
        
        ofPoint dir = ( (v2 - v1).crossed(v3 - v1)).normalized();
        
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }
    for(int i = 0; i < nV; i++) {
        norm[i].normalize();
    }
    
    mesh.clearNormals();
    mesh.addNormals(norm);
}

void PyramidBrush::setup() {
    parameters.setName("PyramidBrush");
    parameters.add(historyMax.set("historyMax", 2000, 100, 10000));
    parameters.add(brushType.set("type line", 0, 0, 1));
    parameters.add(size.set("size", 1, 0, 2));
    parameters.add(opacity.set("opacity", 20, 0, 255));
    parameters.add(lineWidth.set("lineWidth", 0.2, 0., 5.));
    parameters.add(swatch.set("swatch", 0, 0, 12));
    parameters.add(bgColor.set("bgColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(activeColor.set("activeColor", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col1.set("col1", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col2.set("col2", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(col3.set("col3", ofColor(0, 0), ofColor(0, 0), ofColor(255, 255)));
    
    ofPoint v0 = ofPoint( -200, -100, 0 );
    ofPoint v1 = ofPoint( 200, -100, 0 );
    ofPoint v2 = ofPoint( 0, 200, 0 );
    //Pyramid's top vertex
    ofPoint v3 = ofPoint( 0, 0, 500 );
    //Add triangles by its vertices
    mesh.addVertex( v3 ); mesh.addVertex( v2 );
    mesh.addVertex( v0 ); mesh.addVertex( v3 );
    mesh.addVertex( v1 ); mesh.addVertex( v2 );
    mesh.addVertex( v3 ); mesh.addVertex( v0 );
    mesh.addVertex( v1 );
    
    mesh.setupIndicesAuto(); //Set up indices
   setNormals(mesh);
   //light.enable();
    
    mesh.addTexCoord( ofPoint( 100, 100 ) );  //v3
    mesh.addTexCoord( ofPoint( 10, 300 ) );   //v2
    mesh.addTexCoord( ofPoint( 10, 10 ) );    //v0
    mesh.addTexCoord( ofPoint( 100, 100 ) );  //v3
    mesh.addTexCoord( ofPoint( 300, 10 ) );   //v1
    mesh.addTexCoord( ofPoint( 10, 300 ) );   //v2
    mesh.addTexCoord( ofPoint( 100, 100 ) );   //v3
    mesh.addTexCoord( ofPoint( 10, 10 ) );     //v0
    mesh.addTexCoord( ofPoint( 300, 10 ) );    //v1
    ofLoadImage(meshTex, "knife.jpg");
    
    return;
}