#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera();
	OrthographicCamera(float width, float height);
	OrthographicCamera(float width, float height, float zoom);
	OrthographicCamera(glm::vec2 viewScreenSize);
	OrthographicCamera(glm::vec2 viewScreenSize, float zoom);

	void Update() override;
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
};