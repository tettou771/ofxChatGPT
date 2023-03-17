#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");

    inputField.setup();
    inputField.bounds = ofRectangle(20, 20, ofGetWidth() - 40, 30);
    inputField.text = "ここに質問やお話の内容を入力してね";
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    
    inputField.draw();  // 追加

    // 会話履歴を画面に表示する
    int yPos = 70;
    for (const auto &message : chatGPT.conversation) {
        ofDrawBitmapString(message, 20, yPos);
        yPos += 20;
    }
}

void ofApp::keyPressed(int key) {  // 追加
    if (key == OF_KEY_RETURN) {
        std::string message = inputField.text;
        std::string gptResponse = chatGPT.chatWithHistory(message);
        inputField.clear();
    }
}

