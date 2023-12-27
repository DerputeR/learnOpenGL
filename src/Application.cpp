#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "input-handling/BasicInput.h"
#include "shader-loader/ShaderLoader.h"
#include <vector>
#include "stb/stb_image.h"

const int kDefaultWindowWidth = 800;
const int kDefaultWindowHeight = 600;

static std::string vertShaderPath = "resources/shaders/vertex_basic.glsl";
static std::string fragShaderPath = "resources/shaders/fragment_basic.glsl";

static float percent = 0.0f;

static double lastTime = 0.0;
static double currentTime = 0.0;
double deltaTime = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::vector<BasicInput::Key> keys{
	{"quit", GLFW_KEY_ESCAPE},
	{"toggleWireframe", GLFW_KEY_SPACE},
	{"increasePercent", GLFW_KEY_UP},
	{"decreasePercent", GLFW_KEY_DOWN}
};

// todo: figure out how to not be forced to pass a window pointer everywhere
void PollInput(GLFWwindow* window, std::vector<BasicInput::Key>* keys) {
	for (int i = 0; i < keys->size(); i++) {
		keys->at(i).updateState(glfwGetKey(window, keys->at(i).keycode));
	}
}

// todo: give keys an associated convar/action it can execute on process, which will pass its state so that the action can determine how it behaves
void ProcessInput(GLFWwindow* window, std::vector<BasicInput::Key>* keys) {
	if ((*keys)[0].KeyJustPressed()) {
		glfwSetWindowShouldClose(window, true);
	}
	static bool wireframeEnabled = 0;
	if ((*keys)[1].KeyJustPressed()) {
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
	if ((*keys)[2].KeyIsDown()) {
		percent += 1.0f * deltaTime;
		percent = std::min(1.0f, percent);
	}
	if ((*keys)[3].KeyIsDown()) {
		percent -= 1.0f * deltaTime;
		percent = std::max(0.0f, percent);
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
		// x, y, z            // r, g, b          // u, v
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
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
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// Vertex array(attribute) object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind VAO to start tracking state
	glBindVertexArray(VAO);

	// bind VBO and copy vertices array to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// bind EBO and copy indices array to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

	int timeUniformLocation = glGetUniformLocation(shaderProgram, "time");
	int percentUniformLocation = glGetUniformLocation(shaderProgram, "percent");
	int texture0UniformLocation = glGetUniformLocation(shaderProgram, "texture0");
	int texture1UniformLocation = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1i(texture0UniformLocation, 0);
	glUniform1i(texture1UniformLocation, 1);

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
		PollInput(window, &keys);
		ProcessInput(window, &keys);

		// clear last render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
		}
		DrawTriangle(VAO);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}