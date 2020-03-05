#include "VulkanWindow.h"

VulkanWindow::VulkanWindow()
{
	FramebufferResized = false;
	Width = 0;
	Height = 0;
	GLFWindow = nullptr;
}

VulkanWindow::VulkanWindow(unsigned int width, unsigned int height, const char* WindowName)
{
	FramebufferResized = false;
	Width = width;
	Height = height;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWindow = glfwCreateWindow(Width, Height, WindowName, nullptr, nullptr);
	glfwSetWindowUserPointer(GLFWindow, this);
	glfwSetFramebufferSizeCallback(GLFWindow, frameBufferResizeCallBack);
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

void VulkanWindow::frameBufferResizeCallBack(GLFWwindow* Window, int width, int height)
{
	auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(Window));
	app->FramebufferResized = true;
}

