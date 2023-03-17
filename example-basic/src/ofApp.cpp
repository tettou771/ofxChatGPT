#include "ofApp.h"

void ofApp::setup() {
    // Set up the ChatGPT instance with your API key.
    chatGPT.setup("your-api-key");
    
    // Define a single user message for the conversation.
    userMessage = "Can you tell me a funny joke about computers?";
    ofLog() << "User: " << userMessage;

    ofxChatGPT::ErrorCode errorCode;
    // Send the message to ChatGPT and receive a response.
    tie(assistantMessage, errorCode) = chatGPT.chat(userMessage);
    // Check if the response was successful and log the response.
    if (errorCode == ofxChatGPT::Success) {
        ofLog() << "GPT: " << assistantMessage;
    }
}

void ofApp::update() {
    // Update logic for the application (currently empty).
}

void ofApp::draw() {
    // Display the user message and the ChatGPT response on the screen.
    ofDrawBitmapString("User: " + userMessage, 10, 20);
    ofDrawBitmapString("GPT: " + assistantMessage, 10, 40);
}
