#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera
{
private:
    float YAW = -90.0f;
    float PITCH = 0.0f;

    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;

public:

    PerspectiveCamera(glm::vec2 ScreenSize, glm::vec3 position);
    ~PerspectiveCamera();

    virtual void UpdateScreenSize(int NewWidth, int NewHeight);
    virtual void UpdateScreenSize(glm::vec2& ScreenSize);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
    
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void MouseScroll(float yoffset);
    void Update() override;
};
