#pragma once

#include "ofMain.h"

class ofxChatGPT {
public:
    void setup(string apiKey);
    string chat(const string &message);
    string chatWithHistory(const string &message);
    void setModel(const string model);
    vector<string> getModelList();

private:
    string apiKey;
    string modelName;
    vector<ofJson> messages;
    ofHttpResponse sendRequest(const std::string &url, const std::string &body);
};
