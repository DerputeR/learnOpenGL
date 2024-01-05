#include "BasicInput.h"
#include "../math/mathutil.h"

basic_input::KeyStatus basic_input::operator |(const KeyStatus& left, const KeyStatus& right)
{
	return static_cast<KeyStatus>(
		static_cast<char>(left) | static_cast<char>(right)
		);
}


basic_input::KeyStatus basic_input::operator &(const KeyStatus& left, const KeyStatus& right) {
	return static_cast<KeyStatus>(
		static_cast<char>(left) & static_cast<char>(right)
		);
}

basic_input::KeyStatus& basic_input::operator |=(KeyStatus& left, const KeyStatus right) {
	return left = left | right;
}

void basic_input::KeyInput::set_normalized_value(float value) {
	value = clip(value, 0.0f, 1.0f);
	this->prev_normalized_value = this->normalized_value;
	this->normalized_value = value;
	this->prev_status = this->status;
	if (value > activation_threshold) {
		status = KeyStatus::kStatusDown;
		if (!(prev_normalized_value > activation_threshold)) {
			status = status | KeyStatus::kStatusPressed;
		}
	}
	else {
		status = KeyStatus::kStatusUp;
		if (prev_normalized_value > activation_threshold) {
			status = status | KeyStatus::kStatusReleased;
		}
	}
}

float basic_input::KeyInput::get_normalized_value() {
	return this->normalized_value;
}

float basic_input::KeyInput::get_prev_normalized_value() {
	return this->prev_normalized_value;
}

bool basic_input::KeyInput::WasKeyJustPressed() {
	return this->status & KeyStatus::kStatusPressed;
}

bool basic_input::KeyInput::WasKeyJustReleased() {
	return this->status & KeyStatus::kStatusReleased;
}

bool basic_input::KeyInput::IsKeyDown() {
	return this->status & KeyStatus::kStatusDown;
}

bool basic_input::KeyInput::IsKeyUp() {
	return this->status & KeyStatus::kStatusUp;
}

basic_input::Input::Input(float starting_normalized_value)
	: normalized_value{ starting_normalized_value },
	prev_normalized_value{ starting_normalized_value } {
}

basic_input::KeyInput::KeyInput(float starting_normalized_value)
	: Input{ starting_normalized_value } { }

basic_input::KeyInput::KeyInput(float starting_normalized_value, int keycode)
	: Input{ starting_normalized_value }, keycode{ keycode } { }

basic_input::KeyInput::KeyInput(float starting_normalized_value, int keycode, float threshold)
	: Input{ starting_normalized_value }, keycode{ keycode }, activation_threshold{ threshold } { }