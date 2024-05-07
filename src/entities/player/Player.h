#pragma once
#include "../Entity.h"
#include "../Camera.h"
class Player : public Entity
{
	Camera cam;

public:
	// todo: move this to a separate player controller class
	// is fine at 90 but cs does 89 for its own broken reasons
	float pitchMax = 89.0f;
	float pitchMin = -89.0f;

	float sensitivity = 1.3f;
	float m_pitch = 0.022f;
	float m_yaw = 0.022f;
	double mouseX = 0.0;
	double mouseY = 0.0;

	Player();
	Player(glm::vec3 pos, glm::vec3 angles);
	Camera* getCamera();

};

