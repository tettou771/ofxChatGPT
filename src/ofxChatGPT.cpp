#include "ofxChatGPT.h"

void ofxChatGPT::setup(const std::string &apiKey) {
    this->apiKey = apiKey;
}

std::string ofxChatGPT::chat(const std::string &prompt) {
    std::string url = "https://api.openai.com/v1/engines/text-davinci-002/completions";
    std::string body = R"({
        "engine": "text-davinci-002",
        "prompt": ")" + prompt + R"(",
        "max_tokens": 150,
        "n": 1,
        "stop": null,
        "temperature": 0.5
    })";

    ofHttpResponse response = sendRequest(url, body);

    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        return result["choices"][0]["text"].get<std::string>();
    } else {
        ofLogError("ofxChatGPT") << "Error: " << response.status;
        return "";
    }
}

std::string ofxChatGPT::chatWithHistory(const std::string &message) {
    conversation.push_back(message);
    
    std::string conversationText = "";
    for (const auto &msg : conversation) {
        conversationText += msg + "\n";
    }

    std::string gptResponse = chat(conversationText);
    conversation.push_back("GPT: " + gptResponse);

    return gptResponse;
}

ofHttpResponse ofxChatGPT::sendRequest(const std::string &url, const std::string &body) {
    ofHttpRequest request;
    request.url = url;
    request.method = ofHttpRequest::Method::POST;
    request.headers["Content-Type"] = "application/json";
    request.headers["Authorization"] = "Bearer " + apiKey;
    request.body = body;

    ofURLFileLoader loader;
    return loader.handleRequest(request);
}

