#pragma once
#include "ofxChatGPT.h"

// A class for threading chat send and receive
class ChatThread : public ofThread {
public:
    ChatThread();

    void setup(string model, string apiKey);
    void threadedFunction() override;

    // Check if waiting for GPT response
    bool isWaiting();
    bool hasMessage();

    // Register a system message
    void setSystemMessage(string msg);

    // If a request is already in progress, calls to chatAsync, chatWithHistoryAsync, 
    // and regenerateAsync will be ignored. This ensures that only one request is 
    // processed at a time, preventing potential conflicts or overlapping requests.
        
    // Send a chat request asynchronously
    void chatAsync(string msg);

    // Send a request with chat history asynchronously
    void chatWithHistoryAsync(string msg);

    // Resend the request asynchronously
    void regenerateAsync();

    // If a message is available, it can be retrieved
    // Retrieved messages are removed from the list
    tuple<string, ofxChatGPT::ErrorCode> getMessage();

private:
    ofxChatGPT chatGPT;
    string requestingMessage;
    enum RequestType {
        Chat,
        ChatWithHistory,
        Regenerate
    };
    RequestType type;
    ofMutex mutex;
    vector<tuple<string, ofxChatGPT::ErrorCode>> availableMessages;
};
