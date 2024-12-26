#include "Ship.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Ship::Ship(float s, bool outbound) {
    speed = s;
    time = 0.0f;
    rotation = 0.0f;
    if (outbound) {
        GenerateOutboundPath();
    }
    else {
        GenerateInboundPath();
    }
}

Ship::~Ship() {

}

void Ship::GenerateOutboundPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {
        {glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0.0f, 1.0f, 15.0f), glm::vec3(0.0f, 1.0f, 100.0f), glm::vec3(0.0f, 1.0f, 500.0f), glm::vec3(0.0f, 1.0f, 1500.0f)},
        {glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(0.0f, 1.0f, -15.0f), glm::vec3(0.0f, 1.0f, -100.0f), glm::vec3(0.0f, 1.0f, -500.0f), glm::vec3(0.0f, 1.0f, -1500.0f)},
        {glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(15.0f, 1.0f, 0.0f), glm::vec3(100.0f, 1.0f, 0.0f), glm::vec3(500.0f, 1.0f, 0.0f), glm::vec3(1500.0f, 1.0f, 0.0f)},
        {glm::vec3(-10.0f, 1.0f, 0.0f), glm::vec3(-10.0f, 1.0f, 0.0f), glm::vec3(-15.0f, 1.0f, 0.0f), glm::vec3(-100.0f, 1.0f, 0.0f), glm::vec3(-500.0f, 1.0f, 0.0f), glm::vec3(-1500.0f, 1.0f, 0.0f)}
    };

    int choice = std::rand() % predefinedPositions.size();
    switch (choice) {
        case 0:
            rotation = 180.0f;
            break;
        case 1:
            rotation = 0.0f;
            break;
        case 2:
            rotation = -90.0f;
            break;
        case 3:
            rotation = 90.0f;
            break;
    }

    for (const auto& point : predefinedPositions[choice]) {
        waypoints.push_back(point);
    }

    waypoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    waypoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    finalPoint = waypoints[waypoints.size() - 3];
}

void Ship::GenerateInboundPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {
        {glm::vec3(0.0f, 1.0f, 20.0f), glm::vec3(0.0f, 1.0f, 15.0f), glm::vec3(0.0f, 1.0f, 10.0f)},
        {glm::vec3(0.0f, 1.0f, -20.0f), glm::vec3(0.0f, 1.0f, -15.0f), glm::vec3(0.0f, 1.0f, -10.0f)},
        {glm::vec3(20.0f, 1.0f, 0.0f), glm::vec3(15.0f, 1.0f, 0.0f), glm::vec3(10.0f, 1.0f, 0.0f)},
        {glm::vec3(-20.0f, 1.0f, 0.0f), glm::vec3(-15.0f, 1.0f, 0.0f), glm::vec3(-10.0f, 1.0f, 0.0f)}
    };

    int choice = std::rand() % predefinedPositions.size();
    switch (choice) {
        case 0:
            rotation = 0.0f;
            break;
        case 1:
            rotation = 180.0f;
            break;
        case 2:
            rotation = 90.0f;
            break;
        case 3:
            rotation = -90.0f;
            break;
    }


    if (predefinedPositions[choice][0].x > 1 || predefinedPositions[choice][0].x < -1) {
        glm::vec3 midPoint(0.0f, 0.0f, 0.0f);
        glm::vec3 endPoint(0.0f, 0.0f, 0.0f);

        if (predefinedPositions[choice][0].x > 1) {
            midPoint.x = 25.0f;
            endPoint.x = 50.0f;
        }
        else {
            midPoint.x = -25.0f;
            endPoint.x = -50.0f;
        }

        int randomY = (std::rand() % 51) - 25;
        int randomZ = (std::rand() % 51) - 25;

        midPoint.y = randomY / 2;
        midPoint.z = randomZ / 2;
        endPoint.y = randomY;
        endPoint.z = randomZ;

        waypoints.push_back(endPoint);
        waypoints.push_back(midPoint);
    }
    else {
        glm::vec3 midPoint(0.0f, 0.0f, 0.0f);
        glm::vec3 endPoint(0.0f, 0.0f, 0.0f);

        if (predefinedPositions[choice][0].z > 1) {
            midPoint.z = 25.0f;
            endPoint.z = 50.0f;
        }
        else {
            midPoint.z = -25.0f;
            endPoint.z = -50.0f;
        }

        int randomX = (std::rand() % 51) - 25;
        int randomY = (std::rand() % 51) - 25;

        midPoint.x = randomX / 2;
        midPoint.y = randomY / 2;
        endPoint.x = randomX;
        endPoint.y = randomY;

        waypoints.push_back(endPoint);
        waypoints.push_back(midPoint);
    }

    for (const auto& point : predefinedPositions[choice]) {
        waypoints.push_back(point);
    }

    waypoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    waypoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    finalPoint = waypoints[waypoints.size() - 3];
}

glm::vec3 Ship::UpdatePosition(float deltaTime) {
    time += deltaTime;
    float pathProgress = time * speed;
    float segmentProgress = fmod(pathProgress, 1.0f);

    int numPoints = waypoints.size();
    int currentPoint = (int)pathProgress % numPoints;
    int nextPoint = (currentPoint + 1) % numPoints;
    int prevPoint = (currentPoint - 1 + numPoints) % numPoints;
    int nextNextPoint = (currentPoint + 2) % numPoints;

    if (waypoints[currentPoint] == finalPoint) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return 0.5f * (
        (2.0f * waypoints[currentPoint]) +
        (-waypoints[prevPoint] + waypoints[nextPoint]) * segmentProgress +
        (2.0f * waypoints[prevPoint] - 5.0f * waypoints[currentPoint] + 4.0f * waypoints[nextPoint] - waypoints[nextNextPoint]) * (segmentProgress * segmentProgress) +
        (-waypoints[prevPoint] + 3.0f * waypoints[currentPoint] - 3.0f * waypoints[nextPoint] + waypoints[nextNextPoint]) * (segmentProgress * segmentProgress * segmentProgress)
        );
}

float Ship::GetRotation() {
    return rotation;
}