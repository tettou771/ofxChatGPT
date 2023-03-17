#pragma once

#include "ofMain.h"

class ofxChatGPT {
public:
    void setup(const std::string &apiKey);
    std::string chat(const std::string &prompt);
    std::string chatWithHistory(const std::string &message);

private:
    std::string apiKey;
    std::vector<std::string> conversation;
    ofHttpResponse sendRequest(const std::string &url, const std::string &body);
};

