#include "BasicInput.h"

BasicInput::Key::Key(std::string actionName, int glfwKeycode) {
	this->actionName = actionName;
	this->keycode = glfwKeycode;
}

const std::string BasicInput::Key::GetActionName() {
	return this->actionName;
}

void BasicInput::Key::updateState(char state) {
	this->state = (this->state << 1 | state) & kStateMask;
}


bool BasicInput::Key::KeyJustPressed() {
	return !(this->state & kLastStateMask) && (this->state & kCurrentStateMask);
}

bool BasicInput::Key::KeyJustReleased() {
	return (this->state & kLastStateMask) && !(this->state & kCurrentStateMask);
}

bool BasicInput::Key::KeyIsDown() {
	return (this->state & kCurrentStateMask);
}