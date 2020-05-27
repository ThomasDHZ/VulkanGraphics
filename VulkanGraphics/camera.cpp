#include "camera.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;

	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::UpdateKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD: Position += Front * velocity; break;
	case BACKWARD: Position -= Front * velocity; break;
	case LEFT: Position -= Right * velocity; break;
	case RIGHT: Position += Right * velocity; break;
	case UP: Position += Up * velocity; break;
	case DOWN: Position -= Up * velocity; break;
	}
}

void Camera::UpdateMouse(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

float Camera::GetCameraZoom()
{
	return Zoom;
}

Camera& Camera::operator=(const Camera& rhs)
{
	Position = rhs.Position;
	Front = rhs.Front;
	Up = rhs.Up;
	Right = rhs.Up;
	WorldUp = rhs.WorldUp;

	Yaw = rhs.Yaw;
	Pitch = rhs.Pitch;
	MovementSpeed = rhs.MovementSpeed;
	MouseSensitivity = rhs.MouseSensitivity;
	Zoom = rhs.Zoom;

	return *this;
}
