#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera()
{
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
{
	Position = glm::vec3(0.0f);
	Rotation = 0.0f;
	ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void OrthographicCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
}

void OrthographicCamera::SetPosition(const glm::vec2& position)
{
	Position = glm::vec3(position, Position.z);
	UpdateView();
}

void OrthographicCamera::SetPosition(const glm::vec3& position)
{
	Position = position;
	UpdateView();
}

void OrthographicCamera::SetPosition(float x, float y)
{
	Position = glm::vec3(x, y, Position.z);
	UpdateView();
}

void OrthographicCamera::SetPosition(float x, float y, float z)
{
	Position = glm::vec3(x, y, z);
	UpdateView();
}

void OrthographicCamera::SetRotation(float rotation)
{
	Rotation = rotation;
	UpdateView();
}

void OrthographicCamera::UpdateView()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	ViewMatrix = glm::inverse(transform);
}

