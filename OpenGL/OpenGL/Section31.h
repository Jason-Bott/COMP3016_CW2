#pragma once
#include <glm/glm.hpp>
#include "Collider.h"

class Section31
{
public:
	Section31(glm::vec3 shipPosition, glm::mat4 shipRotation);
	glm::vec3 GetPosition();
	glm::mat4 GetRotation();
	bool ExplosionOver(float deltaTime);
	Collider collider;
	int health;
private:
	glm::vec3 position;
	glm::mat4 rotation;
	float explosionTime;
};