#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include "Structs.h"
#include "ForwardRenderer.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "Mesh.h"
#include "VulkanDebugger.h"
#include "VulkanWindow.h"
#include "Model.h"
#include <FileSystem.h>
#include "Camera.h"

class VulkanGraphics
{

private:
	Camera camera;
	VulkanDevice DeviceInfo;
	VulkanDebugger VulkanDebug;
	VulkanWindow Window;

	ForwardRenderer renderer;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	ModelLoader NanoSuit;

	Texture2D texture;
	std::vector<Mesh> meshList;
	//std::vector<Model> ModelList;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	size_t currentFrame = 0;

	bool framebufferResized = false;

	void mainLoop();
	void recreateSwapChain();
	void cleanupSwapChain();
	void updateUniformBuffer(uint32_t currentImage);
	void drawFrame();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	void UpdateKeyboard();
	void UpdateMouse();

public:

	VulkanGraphics();
	VulkanGraphics(unsigned int Width, unsigned int Height, const char* WindowName);
	~VulkanGraphics();
	void run()
	{
		mainLoop();
	}
};

VulkanGraphics::VulkanGraphics()
{
}

VulkanGraphics::VulkanGraphics(unsigned int Width, unsigned int Height, const char* WindowName)
{
	Window = VulkanWindow(Width, Height, WindowName);

	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

		createInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	VulkanDebug.SetUpDebugger(instance);

	renderer = ForwardRenderer(instance, Window.GetWindowPtr());
	DeviceInfo = renderer.UpdateDeviceInfo();

	//NanoSuit = ModelLoader(DeviceInfo, FileSystem::getPath("VulkanGraphics/Models/Nanosuit/nanosuit.obj"));
	

	texture = Texture2D(DeviceInfo, "texture/texture.jpg");
	std::vector<Texture2D> textures = { texture, texture };
	Mesh mesh = Mesh(DeviceInfo, vertices, indices, textures);
	meshList.emplace_back(mesh);

	//ModelList.emplace_back(Model(DeviceInfo, NanoSuit.GetModelMeshs()));

	renderer.createCommandBuffers(meshList);
	renderer.createSyncObjects();
}

VulkanGraphics::~VulkanGraphics()
{
	cleanupSwapChain();

	texture.Destroy();

	vkDestroyBuffer(DeviceInfo.Device, meshList[0].indexBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, meshList[0].indexBufferMemory, nullptr);

	vkDestroyBuffer(DeviceInfo.Device, meshList[0].vertexBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, meshList[0].vertexBufferMemory, nullptr);

	vkDestroyDescriptorSetLayout(DeviceInfo.Device, renderer.descriptorSetLayout, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(DeviceInfo.Device, renderer.renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(DeviceInfo.Device, renderer.imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(DeviceInfo.Device, renderer.inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(DeviceInfo.Device, renderer.commandPool, nullptr);



	VulkanDebug.CleanUp(instance);

	vkDestroyDevice(DeviceInfo.Device, nullptr);

	vkDestroySurfaceKHR(instance, renderer.surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	Window.CleanUp();
}


void VulkanGraphics::mainLoop() {
	while (!glfwWindowShouldClose(Window.GetWindowPtr())) 
	{
		glfwPollEvents();
		drawFrame();
		UpdateMouse();
		UpdateKeyboard();
	}

	vkDeviceWaitIdle(DeviceInfo.Device);
}

void VulkanGraphics::cleanupSwapChain()
{
	renderer.DeleteSwapChain();
	
	//for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++) {
	//	vkDestroyBuffer(DeviceInfo.Device, meshList[0].uniformBuffers[i], nullptr);
	//	vkFreeMemory(DeviceInfo.Device, meshList[0].uniformBuffersMemory[i], nullptr);
	//}

	//vkDestroyDescriptorPool(DeviceInfo.Device, meshList[0].descriptorPool, nullptr);
	for (auto mesh : meshList)
	{
		mesh.ClearSwapChain();
	}
}

void VulkanGraphics::recreateSwapChain() {
	int width = 0, height = 0;
	glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(DeviceInfo.Device);

	cleanupSwapChain();

	renderer.UpdateSwapChain();
	//for (auto mesh : meshList)
	//{
	//	mesh.UpdateSwapChain();
	//}
	meshList[0].UpdateSwapChain();
	renderer.createCommandBuffers(meshList);
}

void VulkanGraphics::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.model = glm::translate(ubo.model, glm::vec3());
	ubo.model = glm::rotate(ubo.model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	ubo.model = glm::scale(ubo.model, glm::vec3(0.2f, 0.2f, 0.2f));
	ubo.view = camera.GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), Window.GetWindowWidth() / (float)Window.GetWindowHeight(), 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;

	for (auto mesh : meshList)
	{
		mesh.UpdateUniformBuffer(ubo, currentImage);
	}
}

void VulkanGraphics::UpdateMouse()
{
	if (glfwGetMouseButton(Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwGetCursorPos(Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
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

		camera.UpdateMouse(xoffset, yoffset);
	}
	else
	{
		firstMouse = true;
	}
}

void VulkanGraphics::UpdateKeyboard()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
		camera.UpdateKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
		camera.UpdateKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
		camera.UpdateKeyboard(LEFT, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
		camera.UpdateKeyboard(RIGHT, deltaTime);
}

void VulkanGraphics::drawFrame() {
	vkWaitForFences(DeviceInfo.Device, 1, &renderer.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(DeviceInfo.Device, renderer.swapChain, UINT64_MAX, renderer.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	updateUniformBuffer(imageIndex);

	if (renderer.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(DeviceInfo.Device, 1, &renderer.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	renderer.imagesInFlight[imageIndex] = renderer.inFlightFences[currentFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { renderer.imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderer.commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderer.renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(DeviceInfo.Device, 1, &renderer.inFlightFences[currentFrame]);

	if (vkQueueSubmit(renderer.graphicsQueue, 1, &submitInfo, renderer.inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { renderer.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(renderer.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

std::vector<const char*> VulkanGraphics::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool VulkanGraphics::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

int main() {
	VulkanGraphics app = VulkanGraphics(1600, 1600, "Vulkan Graphics");

	try {
		app.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

