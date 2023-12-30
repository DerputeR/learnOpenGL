#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 vertexColor;
out vec2 texCoord;

void main() {
	gl_Position = projMatrix * viewMatrix * transform * modelMatrix * vec4(aPos.xyz, 1.0f);
	vertexColor = aColor;
	texCoord = aTexCoord;
}