#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");

    inputField.setup();
    inputField.bounds = ofRectangle(20, 20, ofGetWidth() - 40, 30);
    inputField.text = "Input field";
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(50);
    
    ofSetColor(0);
    ofDrawRectangle(inputField.bounds);
    ofSetColor(255);
    inputField.draw();

    // 会話履歴を画面に表示する
    // 会話履歴を画面に表示する
    stringstream conversationText;
    for (const ofJson &message : chatGPT.getConversation()) {
        conversationText << message["role"] << ": " << message["content"] << "\n";
    }
    ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 70);
}

void ofApp::keyPressed(int key) {  // 追加
    if (key == OF_KEY_RETURN) {
        std::string message = inputField.text;
        std::string gptResponse = chatGPT.chatWithHistory(message);
        inputField.clear();
    }
}

