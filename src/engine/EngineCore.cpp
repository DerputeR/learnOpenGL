#include "EngineCore.h"
#include "../shader-loader/ShaderLoader.h"
#include <iostream>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "../input-handling/UserInputs.h"
#include "../stb/stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	// temporary static variables (will clean up later)
	static glm::vec3 world_up{ 0.0f, 1.0f, 0.0f };
	static glm::vec3 world_right{ 1.0f, 0.0f, 0.0f };
	static glm::vec3 world_forward{ 0.0f, 0.0f, 1.0f };
	static float percent = 1.0f; // 0-1
	static glm::mat4 modelMatrix{ 1.0f };
	static glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3{ 0.0f },	 // cam start position
		-world_forward,      // cam direction is opposite of its physical forward vector
		{0, 1.0f, 0}             // world up direction
	);
	static glm::mat4 transform{ 1.0f }; // single arg appears to just scale the identity matrix; no arg gives null (all 0s) matrix
	static float rotationDeg = 0;
	static glm::vec3 translation{ 0.0f, -0.3f, 0.0f };
	static glm::vec3 scale{ 0.5f, 0.5f, 0.5f };
	static glm::mat4 projectionMatrix
		//= glm::perspective(glm::radians(74.0f), (float)800 / (float)600, 0.1f, 100.0f);
		= glm::ortho(
			-1.0f * (800.0f / 600.0f), // left
			1.0f * (800.0f / 600.0f),  // right
			-1.0f, // bottom
			1.0f,  // top
			-10.0f,   // near
			10.0f  // far
		);

	/// Tri draw call. To remove later
	void DrawTriangle(unsigned int vao, unsigned int triCount) {
		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0); // requires an ebo (indices to a vao)
		glDrawArrays(GL_TRIANGLES, 0, triCount);
		glBindVertexArray(NULL);
	}

	void DemoData::SetupBuffers()
	{
		// Vertex buffer object
		glGenBuffers(1, &vbo);
		// Element buffer object
		glGenBuffers(1, &ebo);
		// Vertex array(attribute) object
		glGenVertexArrays(1, &vao);

		// bind VAO to start tracking state
		glBindVertexArray(vao);

		// bind VBO and copy vertices array to buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// NOTE: the size is in BYTES, not number of elements!
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// ! disabled for now due to headache with cube indices
		//// bind EBO and copy indices array to buffer
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		/* Set vertex attributes pointers.
		Attribute (0) (x, y, z) has (3) non-normalized(GL_FALSE) (GL_FLOAT) elements.
		It starts at (0) byte offset, and repeats every (8 * sizeof(float)) bytes.

		Attribute (1) (r, g, b) has (3) non-normalized(GL_FALSE) (FL_FLOAT) elements.
		It starts at (3 * sizeof(float)) byte offset, and repeats every (8 * sizeof(float)) bytes.

		Attribute (2) (u, v) has (2) non-normalized(GL_FALSE) (FL_FLOAT) elements.
		It starts at (6 * sizeof(float)) byte offset, and repeats every (8 * sizeof(float)) bytes.
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		// enable attribute 0 (x, y, z)
		glEnableVertexAttribArray(0);
		// enable attribute 1 (r, g, b)
		glEnableVertexAttribArray(1);
		// enable attribute 2 (u, v)
		glEnableVertexAttribArray(2);

		// unbind VAO to stop tracking state
		glBindVertexArray(NULL);
	}

	void DemoData::SetupUniforms(unsigned int shader_program)
	{
		time_uniform = glGetUniformLocation(shader_program, "time");
		percent_uniform = glGetUniformLocation(shader_program, "percent");
		texture0_uniform = glGetUniformLocation(shader_program, "texture0");
		texture1_uniform = glGetUniformLocation(shader_program, "texture1");
		transform_uniform = glGetUniformLocation(shader_program, "transform");
		model_matrix_uniform = glGetUniformLocation(shader_program, "modelMatrix");
		view_matrix_uniform = glGetUniformLocation(shader_program, "viewMatrix");
		proj_matrix_uniform = glGetUniformLocation(shader_program, "projMatrix");
	}

	/// Demo data. To remove later
	void DemoData::SetupTextures()
	{
		stbi_set_flip_vertically_on_load(true);
		// Load image data
		for (int i = 0; i < 2; i++) {
			int width;
			int height;
			int nrChannels;
			unsigned char* textureData;
			GLint internalFormat;
			GLenum format;
			switch (i) {
			case 0:
				internalFormat = GL_RGB;
				format = GL_RGB;
				textureData = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
				break;
			case 1:
				internalFormat = GL_RGBA;
				format = GL_RGBA;
				textureData = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
				break;
			default:
				internalFormat = GL_RGB;
				format = GL_RGB;
				textureData = stbi_load("resources/textures/bricktile.png", &width, &height, &nrChannels, 0);
				break;
			}

			if (textureData) {
				textures.push_back(0);
				glGenTextures(1, &textures[i]);
				glBindTexture(GL_TEXTURE_2D, textures[i]);

				// texture wrapping
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// texture filtering
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				// for using with mipmaps, which are ALWAYS smaller than base texture
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Generate texture
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cerr << "Failed to load image!" << std::endl;
			}

			// Free image data
			stbi_image_free(textureData);

		}

		std::cout << "Generated textures with ids: ";
		size_t textureCount = textures.size();
		for (int i = 0; i < textureCount; i++) {
			std::cout << textures[i];
			if (i + 1 != textureCount) {
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	}

	/// Clock

	Clock::Clock(double tick_rate, double time_scale)
		: tick_rate{ tick_rate }, time_scale{ time_scale }
	{ }

	unsigned int Clock::CountAvailableTicks() const
	{
		return ticks_accumulated;
	}

	void Clock::Advance()
	{
		last_time = current_time;
		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		tick_time += time_scale * delta_time;
		while (tick_time >= tick_rate)
		{
			tick_time -= tick_rate;
			ticks_accumulated++;
		}
	}

	void Clock::Tick()
	{
		ticks_accumulated--;
		ticks_elapsed++;
	}

	/// App

    int App::Start()
    {
        std::cout << "Creating window..." << std::endl;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for MacOS

		window = glfwCreateWindow(window_width, window_height, "learnOpenGL", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to generate GLFW window!" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD!" << std::endl;
			return -1;
		}

		glViewport(0, 0, window_width, window_height);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, OnWindowResized);
		glfwSetWindowIconifyCallback(window, OnWindowMinimized);

		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			std::cout << "Raw input is supported, enabling" << std::endl;
		}

		// imgui init
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();
		io->ConfigFlags |= (ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		
		// parse and prepare shader code
		// note: shaders are not part of VAO state
		ShaderLoader::ShaderSources shaderSources = ShaderLoader::ParseShaderSources(ShaderLoader::BASIC_VERT_SHADER_PATH, ShaderLoader::BASIC_FRAG_SHADER_PATH);

		// compile, link, and validate shader program
		shader_program = ShaderLoader::CreateShaderProgram(shaderSources.vertShaderSrc, shaderSources.fragShaderSrc);
		glUseProgram(shader_program);

		// uniforms
		demo_data.SetupUniforms(shader_program);

		// textures
		demo_data.SetupTextures();


		glUniform1i(demo_data.texture0_uniform, 0);
		glUniform1i(demo_data.texture1_uniform, 1);

		glEnable(GL_DEPTH_TEST);

        return this->Loop();
    }

    int App::Loop()
    {
		while (!glfwWindowShouldClose(window)) {
			clock.Advance();

			// start ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// input
			glfwPollEvents();
			PollInput();
			ProcessInput();

			//ImGui::ShowDemoWindow();

			// update matrices
			//UpdateModelMatrix();
			//UpdateViewMatrix();

			// clear last render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// render
			// draw triangles
			if (shader_program) {
				glUseProgram(shader_program);
				glUniform1f(demo_data.time_uniform, static_cast<float>(clock.current_time));
				glUniform1f(demo_data.percent_uniform, percent);
				glUniformMatrix4fv(demo_data.transform_uniform, 1, GL_FALSE, glm::value_ptr(transform));
				glUniformMatrix4fv(demo_data.model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
				glUniformMatrix4fv(demo_data.view_matrix_uniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
				glUniformMatrix4fv(demo_data.proj_matrix_uniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			}
			//DrawTriangle(VAO, sizeof(indices) / sizeof(indices[0]));
			DrawTriangle(demo_data.vao, 36);

			//// Render ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			// check and call events and swap buffers

			glfwSwapBuffers(window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
		return 0;
    }

	Pair<int, int> App::GetWindowResolution()
	{
		return Pair<int, int>{window_width, window_height};
	}

	void App::SetWindowResolution(int width, int height)
	{
		this->window_width = width;
		this->window_height = height;
	}

	void App::SetCursorLock(bool locked)
	{
		if (locked == cursor_locked) return;
		if (locked) { // just locked, so don't snap to where mouse was when not locked
			glfwSetCursorPos(window, window_width / 2.0, window_height / 2.0);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, 0, 0);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		cursor_locked = locked;
	}

	void App::OnWindowResized(GLFWwindow* window, int width, int height) {
		if (width == 0 || height == 0) {
			std::cout << "Ignoring " << width << "x" << height << " resize" << std::endl;
			return;
		}
		App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
		app->window_width = width;
		app->window_height = height;
		glViewport(0, 0, width, height);
	}

	void App::OnWindowMinimized(GLFWwindow* window, int iconified) {
		if (iconified) {
			std::cout << "Window minimized" << std::endl;
		}
		else {
			std::cout << "Window restored" << std::endl;
		}
		// TODO: reduce fps limit to something super small when minimized
	}

	void App::PollInput() {
		// keys
		for (int i = 0; i < user_input::key_inputs.size(); i++) {
			float key_value = static_cast<float>(ImGui::IsKeyDown(static_cast<ImGuiKey>(user_input::key_inputs[i]->keycode)));
			user_input::key_inputs[i]->set_normalized_value(key_value);
		}
		user_input::ProcessInputs(static_cast<float>(clock.delta_time));

		// mouse
		//glfwGetCursorPos(window, &mouseX, &mouseY);
		if (cursor_locked)
		{
			user_input::mouse_x = io->MouseDelta.x;
			user_input::mouse_y = io->MouseDelta.y;
			//glfwSetCursorPos(window, 0, 0);
			//io.MousePos.x = 0.0f; // don't write to this as it screws up the delta calculation
			//io.MousePos.y = 0.0f;
		}
		else {
			user_input::mouse_x = io->MouseDelta.x;
			user_input::mouse_y = io->MouseDelta.y;
		}
	}

	void App::ProcessInput()
	{
		if (user_input::should_quit)
		{
			glfwSetWindowShouldClose(this->window, GLFW_TRUE);
		}
		SetCursorLock(user_input::cursor_locked);
	}
}