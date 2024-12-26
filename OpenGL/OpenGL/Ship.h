#pragma once
#include <vector>
#include <glm/glm.hpp>

class Ship
{
public:
	Ship(float s, bool outbound);
	~Ship();
	void GenerateOutboundPath();
	void GenerateInboundPath();
	glm::vec3 UpdatePosition(float deltaTime);
	float GetRotation();
private:
	std::vector<glm::vec3> waypoints;
	glm::vec3 finalPoint;
	float time;
	float speed;
	float rotation;
};

