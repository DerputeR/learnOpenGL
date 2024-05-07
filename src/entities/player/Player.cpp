#include "Player.h"

Player::Player()
	: Player(glm::vec3{0.0f}, glm::vec3{0.0f})
{
}

Player::Player(glm::vec3 pos, glm::vec3 angles)
	: cam{pos, angles}
{
}

Camera* Player::getCamera()
{
	return &cam;
}
