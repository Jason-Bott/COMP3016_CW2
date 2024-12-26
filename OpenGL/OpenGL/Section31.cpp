#include "Section31.h"

Section31::Section31(glm::vec3 shipPosition, glm::mat4 shipRotation) {
    position = shipPosition;
    rotation = shipRotation;

    collider.min = glm::vec3(position.x - 2.0f, position.y - 2.0f, position.z - 2.0f);
    collider.max = glm::vec3(position.x + 2.0f, position.y + 2.0f, position.z + 2.0f);

    health = 5;
    explosionTime = 0.0f;
}

glm::vec3 Section31::GetPosition() {
    return position;
}

glm::mat4 Section31::GetRotation() {
    return rotation;
}

bool Section31::ExplosionOver(float deltaTime) {
    explosionTime += deltaTime;
    if (explosionTime >= 1.0f) {
        return true;
    }
    return false;
}