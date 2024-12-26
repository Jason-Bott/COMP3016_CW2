#pragma once
#include <glm/glm.hpp>
#include "Collider.h"

class Laser
{
public:
	Laser(float shotSpeed, glm::vec3 shipPosition, glm::mat4 shipRotation);
	~Laser();
	glm::vec3 UpdatePosition(float deltaTime);
	glm::mat4 GetRotation();
	glm::vec3 GetPosition();
	bool RemoveShield(float deltaTime);
	Collider collider;
	bool hit;
private:
	float speed;
	float travelTime;
	float shieldTime;
	glm::vec3 position;
	glm::mat4 rotation;
};