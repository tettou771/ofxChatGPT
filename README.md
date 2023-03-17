# ofxChatGPT

An addon for openFrameworks to interact with ChatGPT within your application. This addon is based on OpenAI's GPT-4 and allows for real-time text-based conversations.

This addon was created by ChatGPT at the request of tettou771.

## Setup

1. Download this repository and place it in your `addons` folder.
2. Obtain the necessary API key and set it in your `ofApp.cpp` file as mentioned in the example.
3. Add `ofxChatGPT` to your `addons.make` file.

## Examples

This addon includes three examples:

1. `example-basic`: A simple example using the `chat()` method with hardcoded input and GPT responses.
2. `example-with-history`: An example using the `chatWithHistory()` method to maintain the conversation history while interacting with GPT.
3. `example-textinputfield`: An example using `ofxTextInputField` to allow users to enter text directly in the application window and send it to GPT.

## Dependencies

This addon requires an internet connection to work, as it communicates with the GPT API to generate responses.

## License

This addon is released under the [MIT License](LICENSE).

