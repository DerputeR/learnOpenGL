#pragma once
#include <inttypes.h>
#include <string>
#include <vector>
#include <functional>

class BasicInput {
	static const char kCurrentStateMask = 1;
	static const char kLastStateMask = 2;
	static const char kStateMask = 3;

public:
	class Key { 
		char state = 0;
		std::string actionName;
	public:
		int keycode = 0;
		Key(std::string actionName, int glfwKeycode);
		void updateState(char state);
		bool KeyJustPressed();
		bool KeyJustReleased();
		bool KeyIsDown();
		const std::string GetActionName();
	};

	//void pollInput(std::vector<Key>* keys, char(*getKeyState)(int keycode));

};

