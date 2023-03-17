#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    // モデルを列挙
    ofLog() << "GPT Model list";
    for (auto model : chatGPT.getModelList()) {
        ofLog() << model;
    }

    // ユーザーからの最初のメッセージ
    userMessage = "Hello, are you ChatGPT?";
    ofLog() << "User: " << userMessage;

    // チャット開始
    assistantMessage = chatGPT.chat(userMessage);
    ofLog() << "GPT: " << assistantMessage;
}

void ofApp::update() {
}

void ofApp::draw() {
    ofDrawBitmapString("User: " + userMessage, 10, 20);
    ofDrawBitmapString("GPT: " + assistantMessage, 10, 40);
}
