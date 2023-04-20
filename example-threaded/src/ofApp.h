#pragma once

#include "ChatThread.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();
	void keyPressed(ofKeyEventArgs& key);

	void sendMessage(string message);
	void regenerate();

	string apiKey;

	ChatThread chat;

	ofJson jQuestion;
	ofJson jResponse;

	bool bError = false;

	string strPrompt;
	int iPrompt = 0;
	string namePrompt;

	vector<pair<string, string> > prompts;

	static string GPT_Prompt_0() {
		return R"(
I want you to act as a music band advertiser. 
You will create a campaign to promote that band.
That campaign consists of 5 short sentences.
These sentences must define the band's career highlights, 
the best albums or the more important musicians members.
The sentences will be short: less than 5 words.
)";
	}

	static string GPT_Prompt_1() {
		return R"(
I want you to act as a music band critic. 
I will pass you a band music name. You will return a list of 10 words.
You will only reply with that words list, and nothing else. 
Words will be sorted starting from less to more relevance.
)";
	}

	static string GPT_Prompt_2() {
		return R"(
I want you to act as a music critic.
As a LastFm maintainer.
I will give you a band name. You will list the 5 more similar bands.
You will only reply that band names list, and nothing else. 
But you must sort that bands, from older to newer. 
)";
	}

	void setPrompt(int index);

	string strBandname;
};
