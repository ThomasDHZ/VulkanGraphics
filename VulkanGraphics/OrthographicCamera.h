#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
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

		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		const glm::vec3& GetPosition() { return Position; }
		float GetRotation() { return Rotation; }

		const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() { return ViewMatrix; }
};

