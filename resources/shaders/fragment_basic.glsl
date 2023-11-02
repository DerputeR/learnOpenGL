#version 330 core
out vec4 FragColor;
uniform float time;

void main() {
	FragColor = vec4(1.0f, 0.5f * (sin(time) + 1.0f), 0.2f, 1.0f);
}