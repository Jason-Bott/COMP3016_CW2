#include "Laser.h"

Laser::Laser(float shotSpeed, glm::vec3 shipPosition, glm::mat4 shipRotation) {
    speed = shotSpeed;
    position = shipPosition;
    rotation = shipRotation;
    travelTime = 0.0f;
    shieldTime = 0.0f;
    hit = false;
}

Laser::~Laser() {

}

glm::vec3 Laser::UpdatePosition(float deltaTime) {
    travelTime += deltaTime;
    if (travelTime >= 10.0f) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 forward = glm::vec3(rotation[2]);
    glm::vec3 displacement = forward * speed * deltaTime;
    position += displacement;

    collider.min = glm::vec3(position.x - 0.01f, position.y - 0.01f, position.z - 0.01f);
    collider.max = glm::vec3(position.x + 0.01f, position.y + 0.01f, position.z + 0.01f);

    return position;
}

glm::mat4 Laser::GetRotation() {
    glm::vec3 direction = glm::normalize(rotation[2]);
    glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, direction));

    glm::mat4 newRotation = glm::mat4(1.0f);
    newRotation[0] = glm::vec4(right, 0.0f);
    newRotation[1] = glm::vec4(up, 0.0f);
    newRotation[2] = glm::vec4(-direction, 0.0f);

    return newRotation;
}

glm::vec3 Laser::GetPosition() {
    return position;
}

bool Laser::RemoveShield(float deltaTime) {
    shieldTime += deltaTime;
    if (shieldTime >= 0.5f) {
        return true;
    }

    return false;
}