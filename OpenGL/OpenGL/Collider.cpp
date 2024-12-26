#include "Collider.h"

Collider::Collider() {
	min = glm::vec3(0.0f);
	max = glm::vec3(1.0f);
}

Collider::Collider(glm::vec3 minCorner, glm::vec3 maxCorner) {
	min = minCorner;
	max = maxCorner;
}

bool Collider::Intersects(Collider other) {
	return (min.x <= other.max.x && max.x >= other.min.x) &&
		   (min.y <= other.max.y && max.y >= other.min.y) &&
		   (min.z <= other.max.z && max.z >= other.min.z);
}