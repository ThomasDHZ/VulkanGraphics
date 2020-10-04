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
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec2 position, float zoom);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position);
	OrthographicCamera(glm::vec2 viewScreenSize, glm::vec3 position, float zoom);

	void Update() override;
	void UpdateScreenSize(glm::vec2& ScreenSize) override;
	void UpdateScreenSize(int NewWidth, int NewHeight) override;
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
};