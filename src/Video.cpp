//
//  Video.cpp
//  DrawingTool
//
//  Created by S l on 11/18/15.
//
//

#include "Video.h"
#ifdef NOT_DEFINED_SHIT

/////// VIDEOFRAME CLASS METHODS //////////

// TODO: add scale, slant and color filter options
void VideoFrame::draw() {
    tex.draw(x,y,z);
}

void VideoFrame::setImg(ofTexture *newVal, float newWidth, float newHeight) {
    ofPixels tmp;
    newVal->readToPixels(tmp );
    tex.loadData(tmp);      // load data will allocate the space needed for the texture
    width = newWidth; 
    height = newHeight;
}

// converts the texture data from VRAM to pixels in RAM
// remmeber the CALLER IS RESPONSIBLE FOR FREEING THE PIXELS IN RAM
ofPixels *VideoFrame::getPixels() {
    ofPixels *out = new ofPixels;
    tex.readToPixels(*out);
    return out;
}

// will ressize the current frame and the frames before and after.
// to resize just the current frame, set before and after to 0.
void VideoFrame::resize(float newWidth, float newHeight, int before, int after) {
    std::list<VideoFrame>::iterator cur = iterator;
    
    width = newWidth;
    height = newHeight;
    
    for(int i = 0; i < before; i++) {
        cur--;
        cur->setWidth(newWidth);
        cur->setWidth(newHeight);
    }
    for(int i = 0; i < after; i++) {
        cur++;
        cur->setWidth(newWidth);
        cur->setWidth(newHeight);
    }
}

// TODO: Implement this.  I think there's no built in for this?
void VideoFrame::resizeTexture(float newWidth, float newHeight, int before, int after) {
/* 
//From http://forum.openframeworks.cc/t/fastest-in-the-sense-of-optimising-way-to-resize-an-image/3876/2

    Image<byte> ImagePyramid::fastResize( Image<byte> input )
    {
        // create image for output
        Image<byte> output(input.size()/2);
    
        int w = input.size().x;
        int h = input.size().y;
        // stride will be 0 if you change this to use openFrameworks getPixels()
        int stride = input.row_stride();
        // ensure even width/height by throwing away the last row/column if necessary
        if ( w%2!= 0 )
            w--;
        if ( h%2!= 0 )
            h--;
        int dstride = output.row_stride();
        int dw = output.size().x;
    
        // separate the kernel processing into columns then rows for efficiency
        int i,j;
        // columns
        for (j=0;j<w;j+=2) {
            // input.data() is the same as ofImage getPixels()
            byte* src = input.data()+j;
            byte* end = src + stride*(h-2);
            byte* dst = output.data()+j/2;
            // pointer arithmetic - this will step through pixel by pixel until
            // src is pointing at the same pixel as end
            while (src != end) {
                byte sum= (byte)(0.25*(src[0]+src[2*w])
                                 + 0.5*src[1*w]);
                // assign sum to the pixel at dst
                *(dst) = sum;
                src += stride*2;
                dst += dstride;
            }
        }
        // rows
        for (i=(h/2)-3;i>=0;i--) {
            // we have already done column update and put the result in output
            // so now read from and write to output only.  
            byte* src = output.data()+i*dstride;  
            byte* end = src + dw-2;  
            while (src != end) {  
                byte sum= (byte)(0.25*(src[0]+src[2])  
                                 + 0.5*src[1]);  
                *(src+1*dstride+1)=sum;  
                ++src;  
            }  
        }  
        
        // resized  
        return output;  
    }  
*/
    return;
}
void VideoFrame::genMipMap() {
    //UNIMPLEMENTED... though maybe all we have to do is ofEnableArbTex() (which allows for using
    // textures with arbitrary dimensions, as opposed to power of two dimensions), then
    // generateMipmap() and enableMipmap(),
    return;
}

/////// VIDEO CLASS METHODS //////////

void Video::setup(string path) {
    
    ofVideoPlayer vid;
    vid.loadMovie(path);
    loadFramelist(&vid);
    vid.close();
    vidPath = path;
}

// path should either be absolute or relative to bin/data
void Video::loadFramelist(ofVideoPlayer *vid) {
    
    VideoFrame *cur= NULL;
    std::list<VideoFrame>::iterator iter_cur = frameList.begin();
    
    // TODO Is this doing deep or shallow copying?
    // What does that mean in the context of VRAM?
    // Should I free the videoPlayer when done?
    while(!vid->getIsMovieDone()) {
        cur = new VideoFrame;
        cur->setImg(vid->getTexture(), vid->getWidth(), vid->getHeight());
        cur->setXYZ(0, 0, 0);
        cur->setIterator(iter_cur);
        iter_cur++;
        frameList.push_back(*cur);
        vid->nextFrame();
    }
}

void Video::chromakey(int frameNum) {
    if (bChromakeyed) {
        return;
    }
    std::list<VideoFrame>::iterator iterator = frameList.begin();
    while(0 <= frameNum) {
        iterator++;
        frameNum--;
    }
    ofPixels *tmp;
    tmp = iterator->getPixels();
    learnBgColor(*tmp);
    bChromakeyed = true;
}
float Video::getThreshold() {
    return threshold;
}
void Video::setThreshold(float newThreshold) {
    threshold = newThreshold;
}

void Video::resize(float newWidth, float newHeight) {
    std::list<VideoFrame>::iterator iterator = frameList.begin();
    std::list<VideoFrame>::iterator end = frameList.end();
    for (; iterator != end; ++iterator) {
        iterator->setWidth(newWidth);
        iterator->setHeight(newHeight);
    }
}

void Video::scale(float scaleFactor) {
    std::list<VideoFrame>::iterator iterator = frameList.begin();
    std::list<VideoFrame>::iterator end = frameList.end();
    for (; iterator != end; ++iterator) {
        iterator->setWidth(iterator->getWidth() * scaleFactor);
        iterator->setHeight(iterator->getHeight() * scaleFactor);
    }
}

void Video::learnBgColor(ofPixelsRef pixelSource) {
    int wXh = pixelSource.getWidth() * pixelSource.getHeight();
    int numPixels = wXh * pixelSource.getBytesPerPixel();
    int r,g,b;
    r=g=b=0;
    for(int i=0; i<numPixels; i+=pixelSource.getBytesPerPixel()) {
        r+=pixelSource[i];
        g+=pixelSource[i+1];
        b+=pixelSource[i+2];
    }
    r/=wXh;
    g/=wXh;
    b/=wXh;
    bgColor.set(r, g, b);
    removeBgFromAllFrames(); // TODO: Maybe remove this and make caller explicitly call this if they want to remove background?
}

// Modified version of https://github.com/underdoeg/ofxChromaKey
// OfxChromaKey is developed by Philip Whitfield (underdoeg)
void Video::removeBgFromAllFrames() {
    int highestKey = 0;
    
    float highest=bgColor[highestKey];
    for(int i=1; i<3; i++) {
        if(bgColor[i]>highest) {
            highestKey=i;
            highest = bgColor[i];
        }
    }
    
    ofTexture *curTex;
    std::list<VideoFrame>::iterator curFrame = frameList.begin();
    ofPixels *pixelSource;
    
    for(int i = 0; i < frameList.size(); i++) {
        pixelSource = curFrame->getPixels();
        int wXh = pixelSource->getWidth() * pixelSource->getHeight();
        int numPixels = wXh * 4;
        unsigned char *pixels = new unsigned char[numPixels];
        ofColor c;
        int index=0;
        
        // TODO: Determine if the end condition for the loop, wXh*3, assumes 3 byte pixel width
        for(int i=0; i<wXh*3; i+=pixelSource->getBytesPerPixel()) {
            c.set((*pixelSource)[i], (*pixelSource)[i+1], (*pixelSource)[i+2]);
            int alpha = 255;
            bool chromaIsHighest = true;
            if(checkForHighest)
                chromaIsHighest = ( c[highestKey] > c[(highestKey+1)%3] && c[highestKey] > c[(highestKey+1)%3] );
            if(fabs(c.getHue()-bgColor.getHue())<threshold && chromaIsHighest)
                alpha=0;
            
            pixels[index] = c.r;
            pixels[index+1] = c.g;
            pixels[index+2] = c.b;
            pixels[index+3] = alpha;
            index+=4;
        }

        curTex = curFrame->getTexMod();
        if(curTex->isAllocated()) {
            curTex->clear();
        }
        curTex->loadData(pixels, pixelSource->getWidth(), pixelSource->getHeight(), OF_IMAGE_COLOR_ALPHA);
        
        curFrame->setUseModified(true);
        curFrame++;
        delete pixels;
    }

}

void Video::restoreBgOnAllFrames() {
    std::list<VideoFrame>::iterator curFrame = frameList.begin();
    
    for(int i = 0; i < frameList.size(); i++) {
        curFrame->setUseModified(false);
    }
}

///////VIDEOMANAGER CLASS////

// TODO: Test preloading videos vs. loading on mouse click


void VideoManager::setup() {
    parameters.setName("VideoManager");
    parameters.add(videoSelect.set("videoSelect", 0, 0, MAX_VIDS));
    parameters.add(numFrames.set("Number of Frames", 30, 0, MAX_FRAMES));
    
    videoPaths.push_back( "/Users/grimes/Downloads/steve_tall.mov");
    videoPaths.push_back( "/Users/grimes/Downloads/steve_tall_stretched.mov");
    
    curVideo = videoPaths[0];
    
    drawMode = ORIG_DIMENSION_ANCHOR_CENTER;
    chromaThresh = 0.0;
    curVideoObj = NULL;
}

void VideoManager::addVideoPath(string path) {
    videoPaths.push_back(path);
}

Video *VideoManager::loadCurVideo() {
    Video *newVid = new Video;
    newVid->setup(curVideo);
    allVideos.push_back(*newVid);
    return newVid;
}

void VideoManager::setVideoPath() {
    int curVidIndex = ofClamp(videoSelect, 0, videoPaths.size() - 1);
    
    curVideo = videoPaths[curVidIndex];
}

void VideoManager::setCurVideoObj() {
    
    curVideoObj = NULL;
}

void VideoManager::mouseDragged(int x, int y, int button) {
    
}

// when the mouse is pressed, create a new video object (using the curVideo variable)
// also note the x, y position of the mouse to aid in drawing the "ghost" frame before actual
// placement of the frame
void VideoManager::mousePressed(int x, int y, int button) {
    init_x = x;
    init_y = y;
    
    curVideoObj = loadCurVideo();
    
    switch (drawMode) {
        case VARIABLE_DIMENSION:    //Scale the video size and orientation to the mouse drag
            break;
        case ORIG_DIMENSION_ANCHOR_LEFT:  //Original dimentsion, anchor point is top left of frame
            break;
        case ORIG_DIMENSION_ANCHOR_CENTER: //Original dimension, anchor pointer is center of frame
            break;
        case TEMLPATE_DIMENSION:        // Use a predefined dimension and orientation
            break;
    }
}

// in this function we know the final position and orientation of the frame, so iterate through
// the VideoFrame objects in the list the Video object maintains, and update the position and
// orientation of each VideoFrame
// Also iterate through the frames of the timeline and add the frame to the VideoFrame list for that frame
// the Layer object of the Timeline maintains a list of VideoFrames on that layer.
void VideoManager::mouseReleased(int x, int y, int button) {
    int final_x = x;
    int final_y = y;
    
    float angleInDegrees = 0.0;
    
    // resize image
    float dist = ofDist(init_x, init_y, final_x, final_y);
    float growth_factor = dist / height;
    
    // calculate slant
    angleInDegrees = atan2((begin->y - end->y), (begin->x - end->x)) * 180.0 / PI;
    angleInDegrees += 270.0;
    cout << angleInDegrees << " is degrees." << endl;
    
    _gif.setScale(growth_factor);
    _gif.setAngleInDegrees(angleInDegrees);
    _gif.draw(begin->x, begin->y, frame_num);
    
    
    curVideoObj = NULL;
}

// reload the 
void VideoManager::layerOrFrameChange() {
    
}
#endif