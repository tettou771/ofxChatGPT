#pragma once

#include "ofMain.h"
#include "ofxChatGPT.h"
#include "ofxTextInputField.h"  // 追加

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);  // 追加

    ofxChatGPT chatGPT;
    ofxTextInputField inputField;  // 追加
};

