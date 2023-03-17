#pragma once

#include "ofMain.h"

class ofxChatGPT {
public:
    // Error codes for various error conditions.
    enum ErrorCode {
        Success = 0,
        InvalidAPIKey,
        NetworkError,
        ServerError,
        RateLimitExceeded,
        TokenLimitExceeded,
        InvalidModel,
        BadRequest,
        Timeout,
        UnknownError
    };

    // Set up the ChatGPT instance with the API key.
    void setup(string apiKey);
    
    // Send a message to ChatGPT and get a response without conversation history.
    tuple<string, ErrorCode> chat(const string &message);
    
    // Send a message to ChatGPT and get a response with conversation history.
    tuple<string, ErrorCode> chatWithHistory(const string &message);
    
    // Get the list of available models from the API.
    tuple<vector<string>, ErrorCode> getModelList();
    
    // Set the model for the ChatGPT instance.
    void setModel(const string model);
    
    // Get the current conversation history.
    vector<ofJson> getConversation();
    
    // Get the error message for a given error code.
    string getErrorMessage(ErrorCode errorCode);

private:
    string apiKey;
    string modelName;
    vector<ofJson> conversation;
    
    // Helper function to send an HTTP request to the specified URL.
    ofHttpResponse sendRequest(const std::string &url, const std::string &body);
    
    // Helper function to parse the error response and return the appropriate error code.
    ErrorCode parseErrorResponse(const ofHttpResponse &response);
};

