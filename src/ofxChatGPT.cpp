#include "ofxChatGPT.h"

const string ofxChatGPT::endpoint = "https://api.openai.com/v1/chat/completions";

ofxChatGPT::ofxChatGPT() {
    temperature = 0.5;
    timeoutSec = 60;
}

void ofxChatGPT::setup(string apiKey) {
    this->apiKey = apiKey;
    modelName = "gpt-3.5-turbo"; // default model
}

void ofxChatGPT::setSystem(const string &message) {
    ofJson json;
    json["role"] = "system";
    json["content"] = message;
    conversation.push_back(json);
}

// Send a message to ChatGPT and get a response without conversation history.
tuple<string, ofxChatGPT::ErrorCode> ofxChatGPT::chat(const string &message) {
    ofJson requestBody;
    requestBody["model"] = modelName;
    requestBody["messages"].push_back({{"role", "user"}, {"content", message}});
    requestBody["temperature"] = 0.5;
    ofLogVerbose("ofxChatGPT") << "SendData: " << requestBody.dump();

    ofHttpResponse response = sendRequest(endpoint, requestBody.dump());

    // Handle the response from ChatGPT.
    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple(result["choices"][0]["message"]["content"].get<std::string>(), Success);
    } else {
        auto errorCode = parseErrorResponse(response);
        if (response.error == "Timeout was reached") {
            errorCode = Timeout;
        }
        ofLogError("ofxChatGPT") << getErrorMessage(errorCode);
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple("", errorCode);
    }
}

// Send a message to ChatGPT and get a response with conversation history.
tuple<string, ofxChatGPT::ErrorCode> ofxChatGPT::chatWithHistory(const string &message) {
    ofJson requestBody;
    requestBody["model"] = modelName;

    // Add the new message to the history
    conversation.push_back({{"role", "user"}, {"content", message}});
    requestBody["messages"] = conversation;
    requestBody["temperature"] = temperature;

    ofLogVerbose("ofxChatGPT") << "SendData: " << requestBody.dump();

    ofHttpResponse response = sendRequest(endpoint, requestBody.dump());

    // Handle the response from ChatGPT.
    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        string assistantReply = result["choices"][0]["message"]["content"].get<std::string>();
        
        // Add the assistant's reply to the history
        conversation.push_back({{"role", "assistant"}, {"content", assistantReply}});

        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple(assistantReply, Success);
    } else {
        auto errorCode = parseErrorResponse(response);
        if (response.error == "Timeout was reached") {
            errorCode = Timeout;
        }

        ofLogError("ofxChatGPT") << getErrorMessage(errorCode);
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple("", errorCode);
    }
}

tuple<string, ofxChatGPT::ErrorCode> ofxChatGPT::chatRegenerate() {
    ofJson requestBody;
    requestBody["model"] = modelName;

    // Remove last assistant message if exists.
    try {
        if (!conversation.empty() && conversation.back()["role"] == "assistant") {
            conversation.erase(conversation.end() - 1);
        }
    }
    catch (exception e) {
        ofLogError("ofxChatGPT") << "conversation JSON " << e.what();
    }
        
    requestBody["messages"] = conversation;
    requestBody["temperature"] = temperature;

    ofLogVerbose("ofxChatGPT") << "SendData: " << requestBody.dump();

    ofHttpResponse response = sendRequest(endpoint, requestBody.dump());

    // Handle the response from ChatGPT.
    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        string assistantReply = result["choices"][0]["message"]["content"].get<std::string>();
        
        // Add the assistant's reply to the history
        conversation.push_back({{"role", "assistant"}, {"content", assistantReply}});

        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple(assistantReply, Success);
    } else {
        auto errorCode = parseErrorResponse(response);
        if (response.error == "Timeout was reached") {
            errorCode = Timeout;
        }
        ofLogError("ofxChatGPT") << getErrorMessage(errorCode);
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple("", errorCode);
    }
}

// Set the model for the ChatGPT instance.
void ofxChatGPT::setModel(const string model) {
    modelName = model;
}

// Get the list of available models from the API.
tuple<vector<string>, ofxChatGPT::ErrorCode> ofxChatGPT::getModelList() {
    string url = "https://api.openai.com/v1/models";
    ofHttpRequest request;
    request.url = url;
    request.method = ofHttpRequest::Method::GET;
    request.headers["Authorization"] = "Bearer " + apiKey;

    ofURLFileLoader loader;
    ofHttpResponse response = loader.handleRequest(request);

    // Handle the response and parse the model list.
    if (response.status == 200) {
        ofJson result = ofJson::parse(response.data.getText());
        vector<string> modelList;

        for (auto& model : result["data"]) {
            modelList.push_back(model["id"].get<std::string>());
        }

        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple(modelList, Success);
    } else {
        auto errorCode = parseErrorResponse(response);
        ofLogError("ofxChatGPT") << getErrorMessage(errorCode);
        ofLogVerbose("ofxChatGPT") << "Data: " << response.data;
        return make_tuple(vector<string>{}, errorCode);
    }
}

// Get the current conversation history.
vector<ofJson> ofxChatGPT::getConversation() {
    return conversation;
}

// Get the error message for a given error code.
string ofxChatGPT::getErrorMessage(ErrorCode errorCode) {
    switch (errorCode) {
        case Success:
            return "Success";
        case InvalidAPIKey:
            return "Invalid API key";
        case NetworkError:
            return "Network error";
        case ServerError:
            return "Server error";
        case RateLimitExceeded:
            return "Rate limit exceeded";
        case TokenLimitExceeded:
            return "Token limit exceeded";
        case InvalidModel:
            return "Invalid model";
        case BadRequest:
            return "Bad request";
        case Timeout:
            return "Timeout";
        default:
            return "Unknown error";
    }
}

void ofxChatGPT::eraseConversation(int beginIndex, int endIndex) {
    if (conversation.empty()) return;
    
    if (beginIndex == endIndex) return;
    
    int begin, end;
    if (beginIndex <= endIndex) {
        begin = MAX(beginIndex, 0);
        end = MIN(endIndex, (int)conversation.size());
    }else{
        end = MAX(beginIndex, 0);
        begin = MIN(endIndex, (int)conversation.size());
    }
    
    int n = end - begin;
    for (int i=0; i<n; ++i) {
        conversation.erase(conversation.begin() + begin);
    }
}

// Helper function to send an HTTP request to the specified URL.
ofHttpResponse ofxChatGPT::sendRequest(const std::string &url, const std::string &body) {
    ofHttpRequest request;
    request.url = url;
    request.method = ofHttpRequest::Method::POST;
    request.headers["Content-Type"] = "application/json";
    request.headers["Authorization"] = "Bearer " + apiKey;
    request.body = body;
    request.timeoutSeconds = timeoutSec;

    ofURLFileLoader loader;
    return loader.handleRequest(request);
}

// Helper function to parse the error response and return the appropriate error code.
ofxChatGPT::ErrorCode ofxChatGPT::parseErrorResponse(const ofHttpResponse &response) {
    int status = response.status;
    if (status == 401) {
        return InvalidAPIKey;
    } else if (status >= 500 && status < 600) {
        return ServerError;
    } else if (status == 429) {
        return RateLimitExceeded;
    } else if (status == 400) {
        ofJson errorJson = ofJson::parse(response.data.getText());
        string errorType = errorJson["error"]["type"].get<std::string>();
        if (errorType == "model_not_found") {
            return InvalidModel;
        } else if (errorType == "too_many_tokens") {
            return TokenLimitExceeded;
        } else {
            return BadRequest;
        }
    } else if (status == 408) {
        return Timeout;
    } else {
        return UnknownError;
    }
}
