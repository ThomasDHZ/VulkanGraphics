#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include "VulkanDebugger.h"
#include "VulkanWindow.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "Camera.h"
#include "ForwardRenderer.h"

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

class VulkanGraphics 
{

private:
	VulkanWindow Window;
	VkInstance instance;
	VulkanDebugger VulkanDebug;

	VulkanDevice DeviceInfo;
	ForwardRenderer renderer;
	Camera camera;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	Texture2D texture;
	Texture2D texture2;

	ModelLoader modelLoader;

	std::vector<Mesh> meshList;
	std::vector<Model> modelList;

	size_t currentFrame = 0;

	bool framebufferResized = false;

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();

	void mainLoop();
	void recreateSwapChain();
	void updateUniformBuffer(uint32_t currentImage);
	void drawFrame();
	void UpdateMouse();
	void UpdateKeyboard();

public:
	VulkanGraphics();
	VulkanGraphics(unsigned int width, unsigned int height, const char* WindowName);
	~VulkanGraphics();

	void VulkanRun();
};
