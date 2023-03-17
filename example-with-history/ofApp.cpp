#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    
    // 新しいメッセージを用意して、chatWithHistory()メソッドを使ってChatGPTと話す
    std::string newMessage = "ここに質問やお話の内容を入力してね";
    std::string gptResponse = chatGPT.chatWithHistory(newMessage);
    
    // 会話履歴を画面に表示する
    std::string conversationText = "";
    for (const auto &message : chatGPT.conversation) {
        conversationText += message + "\n";
    }
    ofDrawBitmapString("会話履歴:\n" + conversationText, 20, 20);
}

