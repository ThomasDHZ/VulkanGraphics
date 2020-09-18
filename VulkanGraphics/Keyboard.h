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

	void UpdateOrtho(GLFWwindow* window, std::shared_ptr<Camera> camera);
};