#version 330 core
uniform float time;
in vec3 vertexColor;
out vec4 FragColor;

void main() {
	// FragColor = vec4(1.0f, 0.5f * (sin(time) + 1.0f), 0.2f, 1.0f);
	FragColor = vec4(vertexColor, 1.0f);
}