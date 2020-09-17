#include "Mouse.h"

void Mouse::Update(GLFWwindow* window, PerspectiveCamera& camera, VulkanRendererSettings settings)
{
	if (!settings.TwoDMode)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &MouseXPos, &MouseYPos);
			if (firstMouse)
			{
				lastX = MouseXPos;
				lastY = MouseYPos;
				firstMouse = false;
			}

			float xoffset = MouseXPos - lastX;
			float yoffset = lastY - MouseYPos;

			lastX = MouseXPos;
			lastY = MouseYPos;

			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else
		{
			firstMouse = true;
		}
	}
}