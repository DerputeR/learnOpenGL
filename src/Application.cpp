#include "Application.h"
#include <iostream>
#include "shader-loader/ShaderLoader.h"
#include <vector>
#include "stb/stb_image.h"
#include "math/mathutil.h"
#include "input-handling/UserInputs.h"

const int kDefaultWindowWidth = 800;
const int kDefaultWindowHeight = 600;

int windowWidth = kDefaultWindowWidth;
int windowHeight = kDefaultWindowHeight;

static std::string vertShaderPath = "resources/shaders/vertex_basic.glsl";
static std::string fragShaderPath = "resources/shaders/fragment_basic.glsl";

static float percent = 0.0f;

static double lastTime = 0.0;
static double currentTime = 0.0;
double deltaTime = 0.0;

// matrices
static glm::mat4 modelMatrix{ 1.0f };
static glm::mat4 viewMatrix{ 1.0f };

static glm::mat4 transform { 1.0f }; // single arg appears to just scale the identity matrix; no arg gives null (all 0s) matrix
static float rotationDeg = 0;
static glm::vec3 translation{ 0.0f, -0.3f, 0.0f };
static glm::vec3 scale{ 0.5f, 0.5f, 0.5f };

static float vFov = 90.0f;
static glm::mat4 projectionMatrix{ };

bool use_perspective = true;
float sensitivity = 1.0f;
float m_pitch = 0.022f;
float m_yaw = 0.022f;
double mouseX = 0.0;
double mouseY = 0.0;

glm::mat4 UpdateProjectionMatrix(bool perspective) {
	if (!perspective) {
		return glm::ortho(
			-1.0f * ((float)windowWidth / (float)windowHeight), // left
			1.0f * ((float)windowWidth / (float)windowHeight),  // right
			-1.0f, // bottom
			1.0f,  // top
			-10.0f,   // near
			10.0f  // far
		);
	}
	else {
		return glm::perspective(glm::radians(vFov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) {
		std::cout << "Ignoring " << width << "x" << height << " resize" << std::endl;
		return;
	}
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
	projectionMatrix = UpdateProjectionMatrix(use_perspective);
}

void window_iconify_callback(GLFWwindow* window, int iconified) {
	if (iconified) {
		std::cout << "Window minimized" << std::endl;
	}
	else {
		std::cout << "Window restored" << std::endl;
	}
}

void UpdateTransformMatrix() {
	// due to the nature of matrix multiplication, the first transformation we want must come last
	// we want to first scale by 0.5, then rotate 90 degrees CCW, then translate
	transform = glm::mat4{ 1.0f };
	transform = glm::translate(transform, translation);
	transform = glm::rotate(transform, glm::radians(rotationDeg), glm::vec3{ 0.0, 0.0, 1.0 });
	transform = glm::scale(transform, scale);
}

void UpdateModelMatrix() {
	//modelMatrix = glm::rotate(glm::mat4{ 1.0f }, (float) currentTime * glm::radians(50.0f), glm::vec3{ 0.5f, 1.0f, 0.0f });
}

void UpdateViewAngles() {
	viewMatrix = glm::mat4{ 1.0f };
	viewMatrix = glm::translate(viewMatrix, glm::vec3{0.0f, 0.0f, -1.0f}); // moving camera backwards (3 units in +z) is = moving world forward (3 units in -z)
}

// todo: figure out how to not be forced to pass a window pointer everywhere
void PollInput(GLFWwindow* window) {
	// keys
	for (int i = 0; i < user_input::key_inputs.size(); i++) {
		float key_value = static_cast<float>(glfwGetKey(window, user_input::key_inputs[i]->keycode));
		user_input::key_inputs[i]->set_normalized_value(key_value);
	}
	user_input::ProcessInputs(static_cast<float>(deltaTime));

	// mouse
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if (user_input::cursor_locked) {
		glfwSetCursorPos(window, 0, 0);
	}
}

// todo: give keys an associated convar/action it can execute on process, which will pass its state so that the action can determine how it behaves
void ProcessInput(GLFWwindow* window) {
	if (user_input::should_quit) {
		glfwSetWindowShouldClose(window, true);
	}

	if (user_input::wireframe_enabled) {
		// built-in wireframe mode
		// NOTE: must use GL_FRONT_AND_BACK in core profile
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (user_input::perspective_enabled != use_perspective) {
		use_perspective = user_input::perspective_enabled;
		projectionMatrix = UpdateProjectionMatrix(use_perspective);
	}
	
	percent = user_input::alpha_value;

	if (rotationDeg != user_input::roll_degrees || scale.x != user_input::model_scale) {
		rotationDeg = user_input::roll_degrees;
		scale = glm::vec3{ user_input::model_scale };
		UpdateTransformMatrix();
	}

	if (user_input::cursor_locked) {
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseX = 0.0f;
		mouseY = 0.0f;
		glfwSetCursorPos(window, 0, 0);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void DrawTriangle(unsigned int vao, unsigned int triCount) {
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, triCount);
	glBindVertexArray(NULL);
}

int main() {
	std::cout << "Creating window..." << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for MacOS

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "learnOpenGL", NULL, NULL);
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

	glViewport(0, 0, windowWidth, windowHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);

	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		std::cout << "Raw input is supported, enabling" << std::endl;
	}

	// temporary vertices for a CUBE
	float vertices[] = {
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
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
			std::cerr << "Failed to load image!"  << std::endl;
		}

		// Free image data
		stbi_image_free(textureData);

	}

	std::cout << "Generated textures with ids: ";
	int textureCount = textures.size();
	for (int i = 0; i < textureCount; i++) {
		std::cout << textures[i];
		if (i + 1 != textureCount) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
	

	// get into habit of drawing CCW
	unsigned int indices[] = {
		1, 0, 3,
		3, 2, 1,
	};

	// Vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Element buffer object
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	// Vertex array(attribute) object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind VAO to start tracking state
	glBindVertexArray(VAO);

	// bind VBO and copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

	// note: shaders are not part of VAO state

	// parse and prepare shader code
	ShaderLoader::ShaderSources shaderSources = ShaderLoader::ParseShaderSources(vertShaderPath, fragShaderPath);

	// compile, link, and validate shader program
	unsigned int shaderProgram = ShaderLoader::CreateShaderProgram(shaderSources.vertShaderSrc, shaderSources.fragShaderSrc);	
	glUseProgram(shaderProgram);

	unsigned int timeUniformLocation = glGetUniformLocation(shaderProgram, "time");
	unsigned int percentUniformLocation = glGetUniformLocation(shaderProgram, "percent");
	unsigned int texture0UniformLocation = glGetUniformLocation(shaderProgram, "texture0");
	unsigned int texture1UniformLocation = glGetUniformLocation(shaderProgram, "texture1");
	unsigned int transformUniformLocation = glGetUniformLocation(shaderProgram, "transform");
	unsigned int modelMatrixUniformLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	unsigned int viewMatrixUniformLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	unsigned int projMatrixUniformLocation = glGetUniformLocation(shaderProgram, "projMatrix");
	glUniform1i(texture0UniformLocation, 0);
	glUniform1i(texture1UniformLocation, 1);

	projectionMatrix = UpdateProjectionMatrix(user_input::perspective_enabled);
	UpdateTransformMatrix();
	UpdateProjectionMatrix();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;

		//for (int i = 0; i < 3; i++) {
		//	vertices[6 * i + 1] += 0.00025f * (sin(time));
		//}

		//glBindVertexArray(VAO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		//glBindVertexArray(NULL);

		// input
		PollInput(window);
		ProcessInput(window);

		// update matrices
		UpdateModelMatrix();
		UpdateViewAngles();

		// clear last render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// I originally did this part in the texture setup step so I'm not doing this right now.
		// But if we had multiple objects and texture sets we wanted to render
		// Then this would be the time to set them
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		
		// render
		// draw triangles
		if (shaderProgram) {
			glUseProgram(shaderProgram);
			glUniform1f(timeUniformLocation, currentTime);
			glUniform1f(percentUniformLocation, percent);
			glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
			glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		}
		//DrawTriangle(VAO, sizeof(indices) / sizeof(indices[0]));
		DrawTriangle(VAO, 36);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}