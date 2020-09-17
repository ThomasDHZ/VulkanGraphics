#pragma once
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "OrthographicCamera.h"
#include "VulkanRenderer.h"
#include "PerspectiveCamera.h"

class Keyboard
{
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

public:

	void Update(GLFWwindow* window, PerspectiveCamera& camera);
	void UpdateOrtho(GLFWwindow* window, OrthographicCamera& camera);
};