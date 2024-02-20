#include "ChatThread.h"

ChatThread::ChatThread() {
    
}

void ChatThread::setup(string model, string apiKey) {
    // setup chatGPT
    chatGPT.setup(apiKey);
    chatGPT.setModel(model);
}

void ChatThread::threadedFunction() {
    tuple<string, ofxChatGPT::ErrorCode> response;
    
    switch (type) {
    case Chat:
        response = chatGPT.chat(requestingMessage);
        break;
    case ChatWithHistory:
        response = chatGPT.chatWithHistory(requestingMessage);
        break;
    case Regenerate:
        response = chatGPT.chatRegenerate();
        break;
    }
    
    mutex.lock();
    availableMessages.push_back(response);
    mutex.unlock();
}

bool ChatThread::isWaiting() {
    return isThreadRunning();
}

bool ChatThread::hasMessage() {
    return availableMessages.size() > 0;
}

void ChatThread::setSystemMessage(string msg) {
    chatGPT.setSystem(msg);
}

void ChatThread::chatAsync(string msg) {
    if (isThreadRunning()) return;
    requestingMessage = msg;
    ofLogNotice("Chat") << "chatAsync " << msg;
    type = Chat;
    startThread();
}

void ChatThread::chatWithHistoryAsync(string msg) {
    if (isThreadRunning()) return;
    requestingMessage = msg;
    ofLogNotice("Chat") << "chatWithHistoryAsync " << msg;
    type = ChatWithHistory;
    startThread();
}

void ChatThread::regenerateAsync() {
    if (isThreadRunning()) return;
    
    type = Regenerate;
    ofLogNotice("Chat") << "regenerateAsync";
    startThread();
}

tuple<string, ofxChatGPT::ErrorCode> ChatThread::getMessage() {
    tuple<string, ofxChatGPT::ErrorCode> result;
    mutex.lock();
    if (availableMessages.size() > 0) {
        ofLogNotice("Chat") << "getMessage";
        result = availableMessages.front();
        availableMessages.erase(availableMessages.begin());
    } else {
        ofLogNotice("Chat") << "No message";
        result = tuple<string, ofxChatGPT::ErrorCode>("", ofxChatGPT::UnknownError);
    }
    mutex.unlock();
    return result;
}
