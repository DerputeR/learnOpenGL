#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input-handling/BasicInput.h"
#include "shader-loader/ShaderLoader.h"
#include <vector>

const int kDefaultWindowWidth = 800;
const int kDefaultWindowHeight = 600;

static std::string vertShaderPath = "resources/shaders/vertex_basic.glsl";
static std::string fragShaderPath = "resources/shaders/fragment_basic.glsl";


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::vector<BasicInput::Key> keys{ {"quit", GLFW_KEY_ESCAPE}, {"toggleWireframe", GLFW_KEY_SPACE} };

// todo: figure out how to not be forced to pass a window pointer everywhere
void PollInput(GLFWwindow* window, std::vector<BasicInput::Key>* keys) {
	for (int i = 0; i < keys->size(); i++) {
		keys->at(i).updateState(glfwGetKey(window, keys->at(i).keycode));
	}
}

// todo: give keys an associated convar/action it can execute on process, which will pass its state so that the action can determine how it behaves
void ProcessInput(GLFWwindow* window, std::vector<BasicInput::Key>* keys) {
	if (keys->at(0).KeyJustPressed()) {
		glfwSetWindowShouldClose(window, true);
	}
	static bool wireframeEnabled = 0;
	if (keys->at(1).KeyJustPressed()) {
		wireframeEnabled = !wireframeEnabled;
		if (wireframeEnabled) {
			// built-in wireframe mode
			// NOTE: must use GL_FRONT_AND_BACK in core profile
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void DrawTriangle(unsigned int vao) {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}

int main() {
	std::cout << "Creating window..." << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for MacOS

	GLFWwindow* window = glfwCreateWindow(kDefaultWindowWidth, kDefaultWindowHeight, "learnOpenGL", NULL, NULL);
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

	glViewport(0, 0, kDefaultWindowWidth, kDefaultWindowHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// temporary vertices for a diamond shape
	float vertices[] = {
		// x, y, z, r, g, b
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // left
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // top
	};

	// get into habit of drawing CCW
	unsigned int indices[] = {
		1, 2, 0,
	};

	// Vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// Vertex array(attribute) object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind VAO to start tracking state
	glBindVertexArray(VAO);

	// bind VBO and copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	// bind EBO and copy indices array to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	/* Set vertex attributes pointers.
	Attribute (0) (x, y, z) has (3) non-normalized(GL_FALSE) (GL_FLOAT) elements.
	It starts at (0) byte offset, and repeats every (6 * sizeof(float)) bytes.

	Attribute (1) (r, g, b) has (3) non-normalized(GL_FALSE) (FL_FLOAT) elements.
	It starts at (3 * sizeof(float)) byte offset, and repeats every (6 * sizeof(float)) bytes.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// enable attribute 0 (x, y, z)
	glEnableVertexAttribArray(0);
	// enable attribute 1 (r, g, b)
	glEnableVertexAttribArray(1);

	// unbind VAO to stop tracking state
	glBindVertexArray(NULL);

	// note: shaders are not part of VAO state


	// parse and prepare shader code
	ShaderLoader::ShaderSources shaderSources = ShaderLoader::ParseShaderSources(vertShaderPath, fragShaderPath);

	// compile, link, and validate shader program
	unsigned int shaderProgram = ShaderLoader::CreateShaderProgram(shaderSources.vertShaderSrc, shaderSources.fragShaderSrc);	

	double time = glfwGetTime();
	int timeUniformLocation = glGetUniformLocation(shaderProgram, "time");

	while (!glfwWindowShouldClose(window)) {
		time = glfwGetTime();
		for (int i = 0; i < 3; i++) {
			vertices[6 * i + 5] = 0.5f * (sin(time) + 1.0f);
		}

		glBindVertexArray(VAO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindVertexArray(NULL);

		// input
		PollInput(window, &keys);
		ProcessInput(window, &keys);

		// clear last render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// render
		// draw triangle
		if (shaderProgram) {
			glUseProgram(shaderProgram);
			glUniform1f(timeUniformLocation, time);
		}
		DrawTriangle(VAO);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}