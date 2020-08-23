#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

//OrthographicCamera::OrthographicCamera()
//{
//}
//
//OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
//	: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
//{
//
//	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
//}
//
//void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
//{
//
//	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
//	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
//}
//
//void OrthographicCamera::RecalculateViewMatrix()
//{
//	
//
//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
//		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
//
//	m_ViewMatrix = glm::inverse(transform);
//	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
//}

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
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position.z -=  5.0f * velocity;
	if (direction == BACKWARD)
		Position.z += 5.0f * velocity;
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
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
	glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));

	ViewMatrix = glm::inverse(transform);
	ViewMatrix = glm::inverse(transform);
}

