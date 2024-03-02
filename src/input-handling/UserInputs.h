#pragma once
#include "BasicInput.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>

namespace user_input {

	// window
	//TrackedValue<bool> cursor_locked = true;
	extern bool cursor_locked;
	extern bool should_quit;
	extern basic_input::KeyInput in_toggle_cursor_lock;
	extern basic_input::KeyInput in_quit;

	// rendering
	extern bool wireframe_enabled;
	extern bool perspective_enabled;
	extern basic_input::KeyInput in_toggle_wireframe;
	extern basic_input::KeyInput in_toggle_perspective;

	// movement
	extern bool move_forward;
	extern bool move_left;
	extern bool move_back;
	extern bool move_right;
	extern basic_input::KeyInput in_move_forward;
	extern basic_input::KeyInput in_move_left;
	extern basic_input::KeyInput in_move_back;
	extern basic_input::KeyInput in_move_right;

	// model
	extern float alpha_value;
	extern float model_scale;
	extern float roll_degrees;
	extern basic_input::KeyInput in_increase_alpha;
	extern basic_input::KeyInput in_decrease_alpha;
	extern basic_input::KeyInput in_roll_ccw;
	extern basic_input::KeyInput in_roll_cw;
	extern basic_input::KeyInput in_scale_up;
	extern basic_input::KeyInput in_scale_down;

	// gui
	extern bool show_debug_overlay;
	extern basic_input::KeyInput in_toggle_debug_overlay;

	extern std::vector<basic_input::KeyInput *> key_inputs;

	void ProcessInputs(float deltaTime);
}