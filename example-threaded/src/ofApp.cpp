#include "ofApp.h"

void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofFile f;
	if (f.doesFileExist("config.json")) {
		ofJson configJson = ofLoadJson("config.json");
		apiKey = configJson["apiKey"].get<string>();//will fail if file do not exist
	}
	else apiKey = "your-api-key";

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

	// create prompts
	prompts.clear();
	prompts.push_back(std::pair<std::string, std::string> { "5 short sentences.", GPT_Prompt_0() });
	prompts.push_back(std::pair<std::string, std::string> {  "10 words list.", GPT_Prompt_1() });
	prompts.push_back(std::pair<std::string, std::string> { "Similar bands", GPT_Prompt_2() });

	setPrompt(0);
}

void ofApp::setPrompt(int index)
{
	if (index > prompts.size() - 1) {
		ofLogError("ofApp") << "Index #" << index << " out of range";
		return;
	}

	iPrompt = index;
	namePrompt = ofToString(prompts[iPrompt].first);
	strPrompt = prompts[iPrompt].second;

	chat.setSystemMessage(strPrompt);

	ofLogNotice("ofApp") << "namePrompt: " << namePrompt;
	ofLogNotice("ofApp") << "strPrompt: " << strPrompt;
}

void ofApp::update() {
	if (chat.hasMessage())
	{
		string gptResponse;
		ofxChatGPT::ErrorCode errorCode;
		tie(gptResponse, errorCode) = chat.getMessage();

		if (errorCode == ofxChatGPT::Success)
		{
			ofJson newGPTMsg;
			newGPTMsg["message"]["role"] = "assistant";
			newGPTMsg["message"]["content"] = gptResponse;

			ofLogVerbose("ofApp") << "GPT: " << newGPTMsg;

			jResponse = newGPTMsg;

			bError = false;
		}
		else
		{
			ofLogError("ofApp") << "ofxChatGPT has an error. " << ofxChatGPT::getErrorMessage(errorCode);
			string message = "Error: " + ofxChatGPT::getErrorMessage(errorCode);

			bError = true;
		}
	}
}

void ofApp::draw()
{
	// Clear Bg: 
	// Blue when waiting. Red if error. 
	ofColor c;
	float v = glm::cos(10 * ofGetElapsedTimef());
	float a1 = ofMap(v, -1, 1, 100, 200, true);
	float a2 = ofMap(v, -1, 1, 8, 16, true);
	bool b = chat.isWaiting();
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
	ofDrawBitmapStringHighlight("conversation:\n" + conversationText.str(), 20, 70);
	 */

	string s = "";
	int x, y, h, i;
	h = 20;
	x = 10;
	y = 20;

	i = 0;
	ofDrawBitmapStringHighlight("QUESTION: \n", x, y + (i++ * h));
	s = ofToString(jQuestion["message"]["role"]);
	ofDrawBitmapStringHighlight(s, x, y + (i++ * h));
	s = ofToString(jQuestion["message"]["content"]);
	ofDrawBitmapStringHighlight(s, x, y + (i++ * h));

	i = 0;
	y = ofGetHeight() / 2;
	ofDrawBitmapStringHighlight("RESPONSE: \n", x, y + (i++ * h));
	s = ofToString(jResponse["message"]["role"]);
	ofDrawBitmapStringHighlight(s, x, y + (i++ * h));
	s = ofToString(jResponse["message"]["content"]);
	ofDrawBitmapStringHighlight(s, x, y + (i++ * h));

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
	s += namePrompt + "\n\n";
	s += "Press ENTER to regenerate.";

	static ofBitmapFont f;
	auto bb = f.getBoundingBox(s, 0, 0);
	y = 20;
	x = ofGetWidth() / 2 - bb.getWidth() / 2;
	ofDrawBitmapStringHighlight(s, x, y);
}

void ofApp::keyPressed(ofKeyEventArgs& key) {
	if (chat.isWaiting()) return;

	if (key.key == '1') { strBandname = "Jane's Addiction"; sendMessage(strBandname); }
	if (key.key == '2') { strBandname = "Fugazi"; sendMessage(strBandname); }
	if (key.key == '3') { strBandname = "Joy Division"; sendMessage(strBandname); }
	if (key.key == '4') { strBandname = "The Smiths";  sendMessage(strBandname); }
	if (key.key == '5') { strBandname = "Radio Futura"; sendMessage(strBandname); }
	if (key.key == '6') { strBandname = "John Frusciante"; sendMessage(strBandname); }
	if (key.key == '7') { strBandname = "Primus"; sendMessage(strBandname); }
	if (key.key == '8') { strBandname = "Kraftwerk"; sendMessage(strBandname); }
	if (key.key == '9') { strBandname = "Portishead"; sendMessage(strBandname); }

	if (key.key == ' ') {//next prompt
		if (iPrompt == 0) iPrompt = 1;
		else if (iPrompt == 1) iPrompt = 2;
		else if (iPrompt == 2) iPrompt = 0;
		setPrompt(iPrompt);
	}

	if (key.key == OF_KEY_RETURN) {//regenerate
		regenerate();
	}
}

void ofApp::sendMessage(string message) {

	ofxChatGPT::ErrorCode errorCode;

	bError = false;

	ofJson newUserMsg;
	newUserMsg["message"]["role"] = "user";
	newUserMsg["message"]["content"] = message;

	ofLogVerbose("ofApp") << "User: " << newUserMsg;

	jQuestion = newUserMsg;
	jResponse = ofJson();

	chat.chatWithHistoryAsync(message);
}

void ofApp::regenerate() {
	ofLogNotice("ofApp") << "Regenerate";

	chat.regenerateAsync();
}

void ofApp::exit() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofJson configJson;
	configJson["apiKey"] = apiKey;
	ofSavePrettyJson("config.json", configJson);
}