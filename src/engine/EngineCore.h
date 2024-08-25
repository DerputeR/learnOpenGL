#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "../misc/TinyStructs.h"
#include <vector>

namespace Engine
{
    /**
     * @brief Simple struct to help keep track of time
     */
    struct Clock
    {
        // Number of seconds since the app was started. Not affected by timescale
        double current_time = 0.0;
        // Number of seconds that has passed between this frame and the last frame. Is not affected by timeScale.
        double delta_time = 0.0;
        // Number of seconds that must pass between physics frames. Should act as a fixed rate.
        double tick_rate = 1.0 / 50.0;
        // Affects the physics frame accumulation. Can also be used to scale other time-sensitive things
        double time_scale = 1.0;

        Clock(double tick_rate = 1.0 / 50.0, double time_scale = 1.0);

        /**
         * @brief Returns the number of physics ticks that can be used
         * @return Number of available physics ticks
         */
        unsigned int CountAvailableTicks() const;

        /**
         * @brief Advance the clock, update deltaTime, and accumulate physics ticks
         */
        void Advance();

        /**
         * @brief Consume a physics tick if any are available
         */
        void Tick();
    private:
        double last_time = 0.0;
        double tick_time = 0.0;
        unsigned int ticks_accumulated = 0;
        unsigned int ticks_elapsed = 0;
    };

    struct DemoData
    {
        // temporary vertices for a vertically stretched cube
        std::vector<float> vertices = {
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
        };

        /* Textures */
        std::vector<unsigned int> textures{};

        /* Buffers */
        unsigned int vbo = 0;
        unsigned int ebo = 0;
        unsigned int vao = 0;

        /* Uniform handles */ 
        unsigned int time_uniform = 0;
        unsigned int percent_uniform = 0;
        unsigned int texture0_uniform = 0;
        unsigned int texture1_uniform = 0;
        unsigned int transform_uniform = 0;
        unsigned int model_matrix_uniform = 0;
        unsigned int view_matrix_uniform = 0;
        unsigned int proj_matrix_uniform = 0;

        void SetupTextures();
    };

    void DrawTriangle(unsigned int vao, unsigned int triCount);

    class App
    {
    public:
        int Start();
        Clock clock;

        Pair<int, int> GetWindowResolution();
        void SetWindowResolution(int width, int height);
        void SetCursorLock(bool locked);
    private:
        GLFWwindow* window = nullptr;
        ImGuiIO* io = nullptr;
        int window_width = 800;
        int window_height = 600;
        bool cursor_locked = false;
        unsigned int shader_program;
        DemoData demo_data;

        int Loop();
        void PollInput();
        void ProcessInput();

        static void OnWindowResized(GLFWwindow* window, int width, int height);
        static void OnWindowMinimized(GLFWwindow* window, int iconified);
    };
}