#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

class OrthographicCamera
{
private:
	float MovementSpeed = 5.0f;
	float Zoom = 0.0f;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::vec2 ViewScreenSize;
	glm::vec3 Position;

	float Rotation;

	void UpdateView();

public:
	float Width;
	float Height;

	OrthographicCamera();
	OrthographicCamera(float width, float height);
	OrthographicCamera(glm::vec2 viewScreenSize);

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void SetPosition(const glm::vec2& position);
	void SetPosition(const glm::vec3& position);
	void SetPosition(float x, float y);
	void SetPosition(float x, float y, float z);
	void SetRotation(float rotation);

	const glm::vec3& GetPosition() { return Position; }
	float GetRotation() { return Rotation; }

	const glm::vec2& GetVuewScreenSize() { return ViewScreenSize; }
	const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() { return ViewMatrix; }
};