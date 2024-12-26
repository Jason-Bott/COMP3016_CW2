#pragma once
#include <vector>
#include <glm/glm.hpp>

class Shuttle
{
public:
	Shuttle(float s, int type);
	~Shuttle();
	void GenerateOutboundPath();
	void GenerateInboundPath();
	void GenerateDrydockPath();
	void GenerateShipPath();
	glm::vec3 UpdatePosition(float deltaTime);
private:
	std::vector<glm::vec3> waypoints;
	glm::vec3 finalPoint;
	float time;
	float speed;
};