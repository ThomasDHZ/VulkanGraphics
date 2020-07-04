#include "Keyboard.h"

void Keyboard::Update(GLFWwindow* window, Camera& camera, VulkanRendererSettings settings)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (settings.TwoDMode)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(DOWN, deltaTime);
		}
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}