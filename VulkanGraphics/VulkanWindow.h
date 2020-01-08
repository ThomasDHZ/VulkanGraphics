#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanWindow
{
private:
	unsigned int Width;
	unsigned int Height;
	GLFWwindow* GLFWindow;
public:
	VulkanWindow();
	VulkanWindow(unsigned int width, unsigned int height, const char* WindowName);
	~VulkanWindow();

	void Update();
	void CleanUp();

	GLFWwindow* GetWindowPtr() { return GLFWindow; }
	unsigned int GetWindowWidth() { return Width; }
	unsigned int GetWindowHeight() { return Height; }
};

