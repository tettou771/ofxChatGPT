#pragma once

#include "ofMain.h"

class ofxChatGPT {
public:
    void setup(string apiKey);
    string chat(const string &message);
    string chatWithHistory(const string &message);
    void setModel(const string model);
    vector<string> getModelList();
    vector<ofJson> getConversation();

private:
    string apiKey;
    string modelName;
    vector<ofJson> conversation;
    ofHttpResponse sendRequest(const std::string &url, const std::string &body);
};
