#include "ofApp.h"

void ofApp::setup() {
    chatGPT.setup("your-api-key");
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    // モデルを列挙
    ofLog() << "GPT Model list";
    for (auto model : chatGPT.getModelList()) {
        ofLog() << model;
    }
    
    // チャット開始
    string userMessage = "Hello, are you ChatGPT?";
    ofLog() << "User: " << userMessage;
    string assistantMessage = chatGPT.chatWithHistory(userMessage);
    ofLog() << "GPT: " << assistantMessage;
    
    userMessage = "Can you answer anything?";
    ofLog() << "User: " << userMessage;
    assistantMessage = chatGPT.chatWithHistory(userMessage);
    ofLog() << "GPT: " << assistantMessage;
    
    userMessage = "That's amazing, I can answer anything too. can i ask you a question?";
    ofLog() << "User: " << userMessage;
    assistantMessage = chatGPT.chatWithHistory(userMessage);
    ofLog() << "GPT: " << assistantMessage;
}

void ofApp::update() {
}

void ofApp::draw() {
    // 会話履歴を画面に表示する
    stringstream conversationText;
    for (const ofJson &message : chatGPT.getConversation()) {
        conversationText << message["role"] << ": " << message["content"] << "\n";
    }
    ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 20);
}

