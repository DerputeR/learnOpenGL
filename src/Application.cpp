#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input-handling/BasicInput.h"
#include <vector>

const int kDefaultWindowWidth = 800;
const int kDefaultWindowHeight = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::vector<BasicInput::Key> keys{ {"quit", GLFW_KEY_ESCAPE}, {"toggleWireframe", GLFW_KEY_SPACE}};

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
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void DrawTriangle() {
	/*glBindVertexArray(VAO);*/
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
		-0.5f, 0.0f, 0.0f, // left
		0.5f, 0.0f, 0.0f,  // right
		0.0f, 0.5f, 0.0f,  // top
		0.0f, -0.5f, 0.0f  // bottom
	};

	// get into habit of drawing CCW
	unsigned int indices[] = {
		1, 2, 0,
		0, 3, 1
	};


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	// bind VAO
	glBindVertexArray(VAO);
	// copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// copy indices array to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// compile vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check if compilation had errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	// compile fragment shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check if compilation had errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	// link shader objects into shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check if linking had errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	// delete individual shader objects after linking to program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// activate shader program
	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(window)) {
		// input
		PollInput(window, &keys);
		ProcessInput(window, &keys);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}