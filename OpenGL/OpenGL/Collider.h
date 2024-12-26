#pragma once
#include <glm/glm.hpp>

class Collider
{
public:
	Collider();
	Collider(glm::vec3 minCorner, glm::vec3 maxCorner);
	bool Intersects(Collider other);
	glm::vec3 min, max;
};