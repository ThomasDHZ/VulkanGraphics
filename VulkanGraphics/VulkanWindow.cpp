#include "VulkanWindow.h"

VulkanWindow::VulkanWindow()
{
}

VulkanWindow::VulkanWindow(unsigned int width, unsigned int height, const char* WindowName)
{
	Width = width;
	Height = height;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWindow = glfwCreateWindow(Width, Height, WindowName, nullptr, nullptr);
}

VulkanWindow::~VulkanWindow()
{

}

void VulkanWindow::Update()
{
	glfwPollEvents();
}

void VulkanWindow::CleanUp()
{
	glfwDestroyWindow(GLFWindow);
	glfwTerminate();
}
