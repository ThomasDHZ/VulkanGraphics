#pragma once
#include "Camera.h"
#include <GLFW\glfw3.h>
#include "VulkanRenderer.h"

class Mouse
{
private:
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

public:
	void Update(GLFWwindow* window, Camera& camera, VulkanRendererSettings settings);
};