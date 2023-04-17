#include "ofApp.h"


void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	//ofJson configJson = ofLoadJson("config.json");
	//string apiKey = configJson["apiKey"].get<string>();
	string apiKey = "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";

	ofxChatGPT chappy;
	chappy.setup(apiKey);
	vector<string> models;
	ofxChatGPT::ErrorCode err;
	tie(models, err) = chappy.getModelList();
	ofLogNotice("ofApp") << "Available OpenAI GPT models:";

	for (auto model : models) {
		if (ofIsStringInString(model, "gpt")) {
			ofLogNotice("ofApp") << model;
		}
	}

	string model = "gpt-3.5-turbo";
	//string model = "gpt-4";

	chat.setup(model, apiKey);

	iPrompt = 0;
	namePrompt = "5 short sentences.";
	prompt = GPT_Prompt_0();
	chat.setSystemMessage(prompt);
}

void ofApp::update() {
	if (chat.hasMessage())
	{
		string gptResponse;
		ofxChatGPT::ErrorCode errorCode;
		tie(gptResponse, errorCode) = chat.getMessage();

		if (errorCode == ofxChatGPT::Success) {
			ofJson newGPTMsg;
			newGPTMsg["message"]["role"] = "assistant";
			newGPTMsg["message"]["content"] = gptResponse;

			ofLogVerbose("ofApp") << "GPT: " << newGPTMsg;

			jResponse = newGPTMsg;

			bError = false;
		}
		else {
			ofLogError("ofApp") << "ofxChatGPT has an error. " << ofxChatGPT::getErrorMessage(errorCode);
			string message = "Error: " + ofxChatGPT::getErrorMessage(errorCode);

			bError = true;
		}
	}
}

void ofApp::draw()
{
	// Bg: blue when waiting. red if error. 
	ofColor c;
	float v = glm::cos(10 * ofGetElapsedTimef());
	float a1 = ofMap(v, -1, 1, 100, 200, true);
	float a2 = ofMap(v, -1, 1, 8, 16, true);
	bool b = chat.isWaiting();
	if (bError) if (b) bError = false;
	if (bError) c = ofColor(a1, 0, 0);
	else if (b) c = ofColor(0, 0, a1);
	else c = ofColor(a2);
	ofClear(c);

	/*
	// Display the conversation on the screen.
	stringstream conversationText;
	// Iterate through the conversation messages and build the display text.
	for (const ofJson &message : chatGPT.getConversation()) {
		conversationText << message["role"] << ": " << message["content"] << "\n";
	}
	// Draw the conversation text on the screen.
	ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 70);
	 */

	string s = "";
	int x, y, h, i;
	h = 20;
	i = 0;
	x = 10;
	y = 20;

	ofDrawBitmapString("QUESTION: \n", x, y + (i++ * h));
	s = ofToString(jQuestion["message"]["role"]);
	ofDrawBitmapString(s, x, y + (i++ * h));
	s = ofToString(jQuestion["message"]["content"]);
	ofDrawBitmapString(s, x, y + (i++ * h));

	i = 0;
	y = ofGetHeight() / 2;
	ofDrawBitmapString("RESPONSE: \n", x, y + (i++ * h));
	s = ofToString(jResponse["message"]["role"]);
	ofDrawBitmapString(s, x, y + (i++ * h));
	s = ofToString(jResponse["message"]["content"]);
	ofDrawBitmapString(s, x, y + (i++ * h));

	s = "";
	s += "Press 1-9 to ask for a MUSIC BAND.\n";
	s += "1 Jane's Addiction\n";
	s += "2 Fugazi\n";
	s += "3 Joy Division\n";
	s += "4 The Smiths\n";
	s += "5 Radio Futura\n";
	s += "6 John Frusciante\n";
	s += "7 Primus\n";
	s += "8 Kraftwerk\n";
	s += "9 Portishead\n\n";

	s += "Press SPACE to swap prompt.\n";
	s += "PROMPT #" + ofToString(iPrompt) + "\n";
	s += namePrompt + "\n";
	static ofBitmapFont f;
	auto bb = f.getBoundingBox(s, 0, 0);
	y = 20;
	x = ofGetWidth() / 2 - bb.getWidth() / 2;
	ofDrawBitmapString(s, x, y);
}

void ofApp::keyPressed(ofKeyEventArgs& key) {
	if (chat.isWaiting()) return;

	if (key.key == '1') sendMessage("Jane's Addiction");
	if (key.key == '2') sendMessage("Fugazi");
	if (key.key == '3') sendMessage("Joy Division");
	if (key.key == '4') sendMessage("The Smiths");
	if (key.key == '5') sendMessage("Radio Futura");
	if (key.key == '6') sendMessage("John Frusciante");
	if (key.key == '7') sendMessage("Primus");
	if (key.key == '8') sendMessage("Kraftwerk");
	if (key.key == '9') sendMessage("Portishead");

	if (key.key == ' ') {//next prompt
		if (iPrompt == 0) iPrompt = 1;
		else if (iPrompt == 1) iPrompt = 2;
		else if (iPrompt == 2) iPrompt = 0;

		if (iPrompt == 0) {
			prompt = GPT_Prompt_0();
			chat.setSystemMessage(prompt);
			namePrompt = "5 short sentences.";
		}
		else if (iPrompt == 1) {
			prompt = GPT_Prompt_1();
			chat.setSystemMessage(prompt);
			namePrompt = "10 words list.";
		}
		else if (iPrompt == 2) {
			prompt = GPT_Prompt_2();
			chat.setSystemMessage(prompt);
			namePrompt = "Similar bands";
		}

		ofLogNotice("ofApp") << "Prompt: " << prompt;
	}
}

void ofApp::sendMessage(string message) {

	ofxChatGPT::ErrorCode errorCode;

	ofJson newUserMsg;
	newUserMsg["message"]["role"] = "user";
	newUserMsg["message"]["content"] = message;

	ofLogVerbose("ofApp") << "User: " << newUserMsg;

	jQuestion = newUserMsg;
	jResponse = ofJson();

	chat.chatWithHistoryAsync(message);
}

//void ofApp::regenerate() {
//	ofLogNotice("ofApp") << "Regenerate";
//
//	chat.regenerateAsync();
//}