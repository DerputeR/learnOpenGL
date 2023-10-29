#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int kDefaultWindowWidth = 800;
const int kDefaultWindowHeight = 600;

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

	return 0;
}