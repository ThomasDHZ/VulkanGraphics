#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
//class OrthographicCamera
//{
//public:
//	OrthographicCamera();
//	OrthographicCamera(float left, float right, float bottom, float top);
//
//	void SetProjection(float left, float right, float bottom, float top);
//
//	const glm::vec3& GetPosition() const { return m_Position; }
//	void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
//
//	float GetRotation() const { return m_Rotation; }
//	void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
//
//	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
//	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
//	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
//private:
//	void RecalculateViewMatrix();
//private:
//	glm::mat4 m_ProjectionMatrix;
//	glm::mat4 m_ViewMatrix;
//	glm::mat4 m_ViewProjectionMatrix;
//
//	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
//	float m_Rotation = 0.0f;
//};

class OrthographicCamera
{
private:
	float MovementSpeed = .5f;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::vec3 Position;

	float Rotation;

	void UpdateView();

public:
	OrthographicCamera();
	OrthographicCamera(float left, float right, float bottom, float top);

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void SetPosition(const glm::vec2& position);
	void SetPosition(const glm::vec3& position);
	void SetPosition(float x, float y);
	void SetPosition(float x, float y, float z);
	void SetRotation(float rotation);

	const glm::vec3& GetPosition() { return Position; }
	float GetRotation() { return Rotation; }

	const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() { return ViewMatrix; }
};