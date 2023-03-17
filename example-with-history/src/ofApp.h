#pragma once

#include "ofMain.h"
#include "ofxChatGPT.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofxChatGPT chatGPT;
};

