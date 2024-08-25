#pragma once
#include <vector>
#include <imgui.h>

namespace basic_input {

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
        ImGuiKey keycode = ImGuiKey_None;

        KeyInput(float starting_normalized_value);
        KeyInput(float starting_normalized_value, ImGuiKey keycode);
        KeyInput(float starting_normalized_value, ImGuiKey keycode, float threshold);

        bool WasKeyJustPressed();
        bool WasKeyJustReleased();
        bool IsKeyDown();
        bool IsKeyUp();

        void set_normalized_value(float value) override;
        float get_normalized_value() override;
        float get_prev_normalized_value() override;
    };
};