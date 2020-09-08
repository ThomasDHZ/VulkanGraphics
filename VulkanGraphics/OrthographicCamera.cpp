#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera()
{
}

OrthographicCamera::OrthographicCamera(float width, float height)
{
	Width = width;
	Height = height;

	Position = glm::vec3(0.0f);
	Rotation = 0.0f;
	ViewScreenSize = glm::vec2(width, height);
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x, ViewScreenSize.x, -ViewScreenSize.y, ViewScreenSize.y, -2.0f, 2.0f);
	ViewMatrix = glm::mat4(1.0f);
}

OrthographicCamera::OrthographicCamera(glm::vec2 viewScreenSize)
{
	Width = viewScreenSize.x;
	Height = viewScreenSize.y;

	Position = glm::vec3(0.0f);
	Rotation = 0.0f;
	ViewScreenSize = viewScreenSize;
	ProjectionMatrix = glm::ortho(-ViewScreenSize.x, ViewScreenSize.x, -ViewScreenSize.y, ViewScreenSize.y, -2.0f, 2.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void OrthographicCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Zoom -=  5.0f * velocity;
	if (direction == BACKWARD)
		Zoom += 5.0f * velocity;
	if (direction == UP)
		Position.y -= 5.0f * velocity;
	if (direction == DOWN)
		Position.y += 5.0f * velocity;
	if (direction == LEFT)
		Position.x += 5.0f * velocity;
	if (direction == RIGHT)
		Position.x -= 5.0f * velocity;
	UpdateView();
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
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));
	ViewMatrix = glm::inverse(transform);

	ProjectionMatrix = glm::ortho(-ViewScreenSize.x + Zoom, ViewScreenSize.x + Zoom, -ViewScreenSize.y + Zoom, ViewScreenSize.y + Zoom, -1.0f, 1.0f);

}

