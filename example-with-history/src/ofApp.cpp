#include "ofApp.h"

void ofApp::setup() {
    // Set up the ChatGPT instance with your API key.
    chatGPT.setup("your-api-key");
    
    // Define a list of user messages for the conversation.
    vector<string> userMessages = {
        "Why don't scientists trust atoms?",
        "What's another funny science joke?"
    };
    
    // Iterate through the user messages and send each message to ChatGPT.
    for (const auto &userMessage : userMessages) {
        ofLog() << "User: " << userMessage;
        string response;
        ofxChatGPT::ErrorCode errorCode;
        // Send the message to ChatGPT and receive a response.
        tie(response, errorCode) = chatGPT.chatWithHistory(userMessage);
        // Check if the response was successful and log the response.
        if (errorCode == ofxChatGPT::Success) {
            ofLog() << "Assistant: " << response;
        }
    }
}

void ofApp::update() {
    // Update logic for the application (currently empty).
}

void ofApp::draw() {
    // Display the conversation on the screen.
    stringstream conversationText;
    // Iterate through the conversation messages and build the display text.
    for (const ofJson &message : chatGPT.getConversation()) {
        conversationText << message["role"] << ": " << message["content"] << "\n";
    }
    // Draw the conversation text on the screen.
    ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 20);
}
