#include "ofxChatGPT.h"

void ofxChatGPT::setup(string apiKey) {
    this->apiKey = apiKey;
    modelName = "gpt-3.5-turbo"; // default
}

std::string ofxChatGPT::chat(const string &message) {
    std::string url = "https://api.openai.com/v1/chat/completions";
    ofJson requestBody;
    requestBody["model"] = modelName;
    requestBody["messages"].push_back({{"role", "user"}, {"content", message}});
    requestBody["temperature"] = 0.5;
    ofLogVerbose("ofxChatGPT") << "SendData: " << requestBody.dump();

    ofHttpResponse response = sendRequest(url, requestBody.dump());

    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return result["choices"][0]["message"]["content"].get<std::string>();
    } else {
        ofLogError("ofxChatGPT") << "Error: " << response.status;
        ofLogError("ofxChatGPT") << "Data: " << response.data;
        return "";
    }
}

std::string ofxChatGPT::chatWithHistory(const string &message) {
    std::string url = "https://api.openai.com/v1/chat/completions";
    ofJson requestBody;
    requestBody["model"] = modelName;

    // Add the new message to the history
    messages.push_back({{"role", "user"}, {"content", message}});
    requestBody["messages"] = messages;
    requestBody["temperature"] = 0.5;

    ofLogVerbose("ofxChatGPT") << "SendData: " << requestBody.dump();

    ofHttpResponse response = sendRequest(url, requestBody.dump());

    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        string assistantReply = result["choices"][0]["message"]["content"].get<std::string>();

        // Add the assistant's reply to the history
        messages.push_back({{"role", "assistant"}, {"content", assistantReply}});

        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return assistantReply;
    } else {
        ofLogError("ofxChatGPT") << "Error: " << response.status;
        ofLogError("ofxChatGPT") << "Data: " << response.data;
        return "";
    }
}

void ofxChatGPT::setModel(const string model) {
    modelName = model;
}

vector<string> ofxChatGPT::getModelList() {
    string url = "https://api.openai.com/v1/models";
    ofHttpRequest request;
    request.url = url;
    request.method = ofHttpRequest::Method::GET;
    request.headers["Authorization"] = "Bearer " + apiKey;

    ofURLFileLoader loader;
    ofHttpResponse response = loader.handleRequest(request);

    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        vector<string> modelList;

        for (auto& model : result["data"]) {
            modelList.push_back(model["id"].get<std::string>());
        }

        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return modelList;
    } else {
        ofLogError("ofxChatGPT") << "Error: " << response.status;
        ofLogError("ofxChatGPT") << "Data: " << response.data;
        return {};
    }
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
