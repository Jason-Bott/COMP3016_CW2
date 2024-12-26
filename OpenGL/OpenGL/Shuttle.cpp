#include "Shuttle.h"
#include <cstdlib>
#include <ctime>

Shuttle::Shuttle(float s, int type) {
    speed = s;
    time = 0.0f;
    if (type == 1) {
        GenerateOutboundPath();
    }
    else if (type == 2) {
        GenerateInboundPath();
    }
    else if (type == 3) {
        GenerateDrydockPath();
    }
    else {
        GenerateShipPath();
    }
}

Shuttle::~Shuttle() {

}

void Shuttle::GenerateOutboundPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {
        {glm::vec3(3.0f, -2.5f, 0.0f), glm::vec3(6.0f, -2.5f, 0.0f), glm::vec3(12.0f, -2.5f, 0.0f)},
        {glm::vec3(-5.0f, -2.5f, 0.0f), glm::vec3(-8.0f, -2.5f, 0.0f), glm::vec3(-14.0f, -2.5f, 0.0f)},
        {glm::vec3(0.5f, -2.5f, -6.0f), glm::vec3(0.5f, -2.5f, -9.0f), glm::vec3(0.5f, -2.5f, -15.0f)},
        {glm::vec3(0.5f, -2.5f, 6.0f), glm::vec3(0.5f, -2.5f, 9.0f), glm::vec3(0.5f, -2.5f, 15.0f)},
        {glm::vec3(-0.5f, -2.5f, -6.0f), glm::vec3(-0.5f, -2.5f, -9.0f), glm::vec3(-0.5f, -2.5f, -15.0f)},
        {glm::vec3(-0.5f, -2.5f, 6.0f), glm::vec3(-0.5f, -2.5f, 9.0f), glm::vec3(-0.5f, -2.5f, 15.0f)}
    };

    int choice = std::rand() % predefinedPositions.size();
    for (const auto& point : predefinedPositions[choice]) {
        waypoints.push_back(point);
    }

    if (waypoints[0].x > 1 || waypoints[0].x < -1) {
        glm::vec3 midPoint(0.0f, 0.0f, 0.0f);
        glm::vec3 endPoint(0.0f, 0.0f, 0.0f);

        if (waypoints[0].x > 1) {
            midPoint.x = 25.0f;
            endPoint.x = 50.0f;
        }
        else {
            midPoint.x = -25.0f;
            endPoint.x = -50.0f;
        }

        int randomY = (std::rand() % 101) - 50;
        int randomZ = (std::rand() % 101) - 50;

        midPoint.y = randomY / 2;
        midPoint.z = randomZ / 2;
        endPoint.y = randomY;
        endPoint.z = randomZ;

        waypoints.push_back(midPoint);
        waypoints.push_back(endPoint);

        if (waypoints[0].x > 1) {
            endPoint.x++;
            waypoints.push_back(endPoint);
            endPoint.x++;
            waypoints.push_back(endPoint);
        }
        else {
            endPoint.x--;
            waypoints.push_back(endPoint);
            endPoint.x--;
            waypoints.push_back(endPoint);
        }
    }
    else {
        glm::vec3 midPoint(0.0f, 0.0f, 0.0f);
        glm::vec3 endPoint(0.0f, 0.0f, 0.0f);

        if (waypoints[0].z > 1) {
            midPoint.z = 25.0f;
            endPoint.z = 50.0f;
        }
        else {
            midPoint.z = -25.0f;
            endPoint.z = -50.0f;
        }

        int randomX = (std::rand() % 101) - 50;
        int randomY = (std::rand() % 101) - 50;

        midPoint.x = randomX / 2;
        midPoint.y = randomY / 2;
        endPoint.x = randomX;
        endPoint.y = randomY;

        waypoints.push_back(midPoint);
        waypoints.push_back(endPoint);

        if (waypoints[0].z > 1) {
            endPoint.z++;
            waypoints.push_back(endPoint);
            endPoint.z++;
            waypoints.push_back(endPoint);
        }
        else {
            endPoint.z--;
            waypoints.push_back(endPoint);
            endPoint.z--;
            waypoints.push_back(endPoint);
        }
    }

    finalPoint = waypoints[waypoints.size() - 3];
}

void Shuttle::GenerateInboundPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {
        {glm::vec3(12.0f, -2.5f, 0.0f), glm::vec3(6.0f, -2.5f, 0.0f), glm::vec3(3.0f, -2.5f, 0.0f)},
        {glm::vec3(-14.0f, -2.5f, 0.0f), glm::vec3(-8.0f, -2.5f, 0.0f), glm::vec3(-5.0f, -2.5f, 0.0f)},
        {glm::vec3(0.5f, -2.5f, -15.0f), glm::vec3(0.5f, -2.5f, -9.0f), glm::vec3(0.5f, -2.5f, -6.0f)},
        {glm::vec3(0.5f, -2.5f, 15.0f), glm::vec3(0.5f, -2.5f, 9.0f), glm::vec3(0.5f, -2.5f, 6.0f)},
        {glm::vec3(-0.5f, -2.5f, -15.0f), glm::vec3(-0.5f, -2.5f, -9.0f), glm::vec3(-0.5f, -2.5f, -6.0f)},
        {glm::vec3(-0.5f, -2.5f, 15.0f), glm::vec3(-0.5f, -2.5f, 9.0f), glm::vec3(-0.5f, -2.5f, 6.0f)}
    };

    int choice = std::rand() % predefinedPositions.size();

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

        int randomY = (std::rand() % 101) - 50;
        int randomZ = (std::rand() % 101) - 50;

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

        int randomX = (std::rand() % 101) - 50;
        int randomY = (std::rand() % 101) - 50;

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

    waypoints.push_back(glm::vec3(0.0f, -2.5f, 0.0f));
    waypoints.push_back(glm::vec3(0.0f, -2.5f, 0.0f));

    finalPoint = waypoints[waypoints.size() - 3];
}

void Shuttle::GenerateDrydockPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {
        {glm::vec3(24.8f, -5.0f, -0.5f), glm::vec3(24.8f, -5.0f, 0.0f), glm::vec3(24.8f, -5.0f, 1.0f), glm::vec3(24.8f, -5.0f, 3.0f), glm::vec3(24.8f, -5.0f, 5.0f), glm::vec3(20.0f, -4.0f, 15.0f), glm::vec3(10.0f, -3.0f, 15.0f), glm::vec3(0.5f, -2.0f, 10.0f), glm::vec3(0.5f, -2.0f, 7.0f), glm::vec3(0.5f, -2.0f, 3.0f), glm::vec3(0.5f, -2.0f, 0.0f), glm::vec3(24.8f, -5.0f, -0.5f)},
        {glm::vec3(0.5f, -2.0f, 0.0f), glm::vec3(0.5f, -2.0f, 3.0f), glm::vec3(0.5f, -2.0f, 7.0f), glm::vec3(0.5f, -2.0f, 10.0f), glm::vec3(10.0f, -3.0f, 15.0f), glm::vec3(20.0f, -4.0f, 15.0f), glm::vec3(24.8f, -5.0f, 5.0f), glm::vec3(24.8f, -5.0f, 3.0f), glm::vec3(24.8f, -5.0f, 1.0f), glm::vec3(24.8f, -5.0f, 0.0f), glm::vec3(24.8f, -5.0f, -0.5f), glm::vec3(0.5f, -2.0f, 0.0f)}
    };

    int choice = std::rand() % predefinedPositions.size();
    for (const auto& point : predefinedPositions[choice]) {
        waypoints.push_back(point);
    }

    finalPoint = waypoints[waypoints.size() - 3];
}

void Shuttle::GenerateShipPath() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<glm::vec3>> predefinedPositions = {

    };

    int choice = std::rand() % predefinedPositions.size();
    for (const auto& point : predefinedPositions[choice]) {
        waypoints.push_back(point);
    }
}

glm::vec3 Shuttle::UpdatePosition(float deltaTime) {
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