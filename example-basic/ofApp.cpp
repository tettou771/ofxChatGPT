#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    
    // プロンプトを用意して、ChatGPTに質問を投げる
    std::string prompt = "ここに質問やお話の内容を入力してね";
    std::string gptResponse = chatGPT.chat(prompt);
    
    // レスポンスを画面に表示する
    ofDrawBitmapString("GPTの回答: " + gptResponse, 20, 20);
}

