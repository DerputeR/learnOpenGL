#version 330 core
in vec4 vertexColor;
out vec4 FragColor;
uniform float time;

void main() {
//	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
//	FragColor = vertexColor;
	FragColor = vec4(vertexColor.x, 0.5f*(sin(time) + 1.0f), vertexColor.z, 1.0f);
}