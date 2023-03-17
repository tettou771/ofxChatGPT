#include "ofApp.h"

void ofApp::setup() {
    // Set up the ChatGPT instance with your API key.
    chatGPT.setup("your-api-key");

    // Make a input field
    inputField.setup();
    inputField.bounds = ofRectangle(20, 20, ofGetWidth() - 40, 30);
    inputField.text = "Input field";
}

void ofApp::update() {
    // Update logic for the application (currently empty).
}

void ofApp::draw() {
    // background (black)
    ofBackground(0);
    
    // Display input area (gray)
    ofSetColor(50);
    ofDrawRectangle(inputField.bounds);
    ofSetColor(255);
    inputField.draw();

    // Display the conversation on the screen.
    stringstream conversationText;
    // Iterate through the conversation messages and build the display text.
    for (const ofJson &message : chatGPT.getConversation()) {
        conversationText << message["role"] << ": " << message["content"] << "\n";
    }
    // Draw the conversation text on the screen.
    ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 70);
}

void ofApp::keyPressed(int key) {  // 追加
    if (key == OF_KEY_RETURN) {
        ofxChatGPT::ErrorCode errorCode;
        string message = inputField.text;
        string gptResponse;
        tie(gptResponse, errorCode) = chatGPT.chatWithHistory(message);
        inputField.clear();
    }
}

