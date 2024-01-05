#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

glm::mat4 UpdateProjectionMatrix(bool perspective = true);
void UpdateTransformMatrix();
void UpdateModelMatrix();
void UpdateViewAngles();

void PollInput(GLFWwindow* window);
void ProcessInput(GLFWwindow *window);

