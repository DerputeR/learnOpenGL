#include "UserInputs.h"
#include <algorithm>

namespace user_input {

	bool cursor_locked = true;
	bool should_quit = false;
	bool wireframe_enabled = false;
	bool perspective_enabled = true;
	bool move_forward = false;
	bool move_left = false;
	bool move_back = false;
	bool move_right = false;
	float alpha_value = 0.0f;
	float model_scale = 1.0f;
	float roll_degrees = 0.0f;
	bool show_debug_overlay = true;

	basic_input::KeyInput in_toggle_cursor_lock{ 0.0f, GLFW_KEY_C };
	basic_input::KeyInput in_quit{ 0.0f, GLFW_KEY_ESCAPE };
	basic_input::KeyInput in_toggle_wireframe{ 0.0f, GLFW_KEY_TAB };
	basic_input::KeyInput in_toggle_perspective{ 0.0f, GLFW_KEY_F5 };
	basic_input::KeyInput in_move_forward{ 0.0f, GLFW_KEY_W };
	basic_input::KeyInput in_move_left{ 0.0f, GLFW_KEY_A };
	basic_input::KeyInput in_move_back{ 0.0f, GLFW_KEY_S };
	basic_input::KeyInput in_move_right{ 0.0f, GLFW_KEY_D };
	basic_input::KeyInput in_increase_alpha{ 0.0f, GLFW_KEY_UP };
	basic_input::KeyInput in_decrease_alpha{ 0.0f, GLFW_KEY_DOWN };
	basic_input::KeyInput in_roll_ccw{ 0.0f, GLFW_KEY_LEFT };
	basic_input::KeyInput in_roll_cw{ 0.0f, GLFW_KEY_RIGHT };
	basic_input::KeyInput in_scale_up{ 0.0f, GLFW_KEY_EQUAL };
	basic_input::KeyInput in_scale_down{ 0.0f, GLFW_KEY_MINUS };
	basic_input::KeyInput in_toggle_debug_overlay{ 0.0f, GLFW_KEY_F3 };

	std::vector<basic_input::KeyInput*> key_inputs{
		&in_toggle_cursor_lock, &in_quit,
		&in_toggle_wireframe, &in_toggle_perspective,
		&in_move_forward, &in_move_left, &in_move_back, &in_move_right,
		&in_increase_alpha, &in_decrease_alpha,
		&in_roll_ccw, &in_roll_cw,
		&in_scale_up, &in_scale_down,
		&in_toggle_debug_overlay
	};

	void ProcessInputs(float deltaTime) {
		if (in_quit.WasKeyJustPressed()) {
			should_quit = true;
		}
		if (in_toggle_wireframe.WasKeyJustPressed()) {
			wireframe_enabled = !wireframe_enabled;
		}
		if (in_increase_alpha.IsKeyDown()) {
			alpha_value += in_increase_alpha.get_normalized_value() * deltaTime;
			alpha_value = std::min(1.0f, alpha_value);
		}
		if (in_decrease_alpha.IsKeyDown()) {
			alpha_value -= in_decrease_alpha.get_normalized_value() * deltaTime;
			alpha_value = std::max(0.0f, alpha_value);
		}
		if (in_roll_ccw.IsKeyDown()) {
			roll_degrees += 90.0f * deltaTime;
		}
		if (in_roll_cw.IsKeyDown()) {
			roll_degrees -= 90.0f * deltaTime;
		}
		if (in_scale_up.IsKeyDown()) {
			model_scale += in_scale_up.get_normalized_value() * deltaTime;
		}
		if (in_scale_down.IsKeyDown()) {
			model_scale -= in_scale_down.get_normalized_value() * deltaTime;
		}
		if (in_toggle_perspective.WasKeyJustPressed()) {
			perspective_enabled = !perspective_enabled;
		}
		if (in_toggle_cursor_lock.WasKeyJustPressed()) {
			cursor_locked = !cursor_locked;
		}
		if (in_toggle_debug_overlay.WasKeyJustPressed()) {
			show_debug_overlay = !show_debug_overlay;
		}
		move_forward = in_move_forward.IsKeyDown();
		move_back = in_move_back.IsKeyDown();
		move_left = in_move_left.IsKeyDown();
		move_right = in_move_right.IsKeyDown();
	}
}