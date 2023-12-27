#version 330 core
uniform float time;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float percent;

in vec3 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

void main() {
	// FragColor = vec4(1.0f, 0.5f * (sin(time) + 1.0f), 0.2f, 1.0f);
	vec2 scaledCoord = 5f * texCoord;
	vec4 timeColor = vec4(vertexColor.y * 0.5f * (cos(time) + 1.0f), vertexColor.y * 0.5f * (sin(time) + 1.0f), vertexColor.z * 0.5f * (-sin(time) + 1.0f), 1.0f);
	// alpha value here seems to not have an effect
	// FragColor = vec4(vertexColor, 1.0f) * vec4((texCoord.x + texCoord.y) / 2.0f);
	// FragColor = texture(texture0, texCoord);
	// FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
	vec4 texSample0 = texture(texture0, scaledCoord);
	vec4 texSample1 = texture(texture1, vec2(1 - scaledCoord.x, scaledCoord.y));
	FragColor = mix(texSample0, texSample1, percent * texSample1.w) * timeColor;
}