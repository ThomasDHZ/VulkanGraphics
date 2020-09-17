#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Camera
{
protected:
    float MovementSpeed = 5.0f;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::vec2 ViewScreenSize;

public:
    Camera();
    ~Camera();

    const glm::vec2& GetViewScreenSize() { return ViewScreenSize; }
    const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() { return ViewMatrix; }
};