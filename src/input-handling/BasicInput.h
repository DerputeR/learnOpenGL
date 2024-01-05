#pragma once
#include <vector>

namespace basic_input {

#ifdef GLFW_KEY_UNKNOWN
	const int kKeyUnknown = GLFW_KEY_UNKNOWN;
#else GLFW_KEY_UNKNOWN
	const int kKeyUnknown = -1;
#endif

	enum KeyStatus : char {
		kStatusUnknown = 0,
		kStatusUp = 1 << 0,
		kStatusReleased = 1 << 1,
		kStatusDown = 1 << 2,
		kStatusPressed = 1 << 3
	};

	KeyStatus operator |(const KeyStatus& left, const KeyStatus& right);
	KeyStatus operator &(const KeyStatus& left, const KeyStatus& right);
	KeyStatus& operator |=(KeyStatus& left, const KeyStatus right);

	class Input {
	protected:
		float prev_normalized_value;
		float normalized_value;
	public:
		Input(float starting_normalized_value);
		virtual void set_normalized_value(float value) = 0;
		virtual float get_normalized_value() = 0;
		virtual float get_prev_normalized_value() = 0;
	};

	class KeyInput : public Input {
		KeyStatus status = KeyStatus::kStatusUp;
		KeyStatus prev_status = KeyStatus::kStatusUp;
	public:
		float activation_threshold = 0.0f;
		int keycode = kKeyUnknown;

		KeyInput(float starting_normalized_value);
		KeyInput(float starting_normalized_value, int keycode);
		KeyInput(float starting_normalized_value, int keycode, float threshold);

		bool WasKeyJustPressed();
		bool WasKeyJustReleased();
		bool IsKeyDown();
		bool IsKeyUp();

		void set_normalized_value(float value) override;
		float get_normalized_value() override;
		float get_prev_normalized_value() override;
	};

	//template <typename T>
	//class TrackedValue {
	//	std::vector<T> value_history{ 2, {} };
	//	int current = 0;
	//public:
	//	TrackedValue() {
	//		value_history[0] = T{};
	//	}
	//	TrackedValue(T init) {
	//		value_history[0] = init;
	//	}
	//	T get() const {
	//		return value_history[current];
	//	}
	//	T get(int index) const {
	//		index = index % value_history.size();
	//		return value_history[current];
	//	}
	//	void set(T value) {
	//		current = (current + 1) % value_history.size();
	//		value_history[current] = value;
	//	}
	//	void operator=(const T &right) {
	//		set(right);
	//	}
	//};
};