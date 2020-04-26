//#include "VulkanGraphics.h"
//
//#include <fstream>
//#include <stdexcept>
//#include <algorithm>
//#include <chrono>
//#include <vector>
//#include <cstring>
//#include <cstdlib>
//#include <cstdint>
//#include <array>
//#include <optional>
//#include <set>
//#include <FileSystem.h>
//
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//#include "Image.h"
//
//VulkanGraphics::VulkanGraphics()
//{
//
//}
//
//VulkanGraphics::VulkanGraphics(unsigned int width, unsigned int height, const char* WindowName)
//{
//	// DeviceInfo.Window = VulkanWindow(width, height, WindowName);
//
//	if (enableValidationLayers && !checkValidationLayerSupport()) {
//		throw std::runtime_error("validation layers requested, but not available!");
//	}
//
//	VkApplicationInfo appInfo = {};
//	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//	appInfo.pApplicationName = "Vulkan Graphics";
//	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//	appInfo.pEngineName = "No Engine";
//	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//	appInfo.apiVersion = VK_API_VERSION_1_2;
//
//	VkInstanceCreateInfo createInfo = {};
//	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//	createInfo.pApplicationInfo = &appInfo;
//
//	auto extensions = getRequiredExtensions();
//	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//	createInfo.ppEnabledExtensionNames = extensions.data();
//
//	if (enableValidationLayers)
//	{
//		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
//		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);
//
//		createInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
//		createInfo.ppEnabledLayerNames = validationLayers.data();
//		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
//	}
//	else
//	{
//		createInfo.enabledLayerCount = 0;
//		createInfo.pNext = nullptr;
//	}
//
//	//if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//	//	throw std::runtime_error("failed to create instance!");
//	//}
//
//	//VulkanDebug.SetUpDebugger(vulkanDevice2.Instance);
//
//
//
//	//renderer = Renderer2D(vulkanDevice2.Instance, vulkanDevice2.Window.GetWindowPtr());
//	//DeviceInfo = renderer.UpdateDeviceInfo();
//
//	//CubeMapLayout layout;
//	//layout.Left = "texture/skybox/left.jpg";
//	//layout.Right = "texture/skybox/right.jpg";
//	//layout.Top = "texture/skybox/top.jpg";
//	//layout.Bottom = "texture/skybox/bottom.jpg";
//	//layout.Back = "texture/skybox/back.jpg";
//	//layout.Front = "texture/skybox/front.jpg";
//	//cubeTexture = CubeMapTexture(DeviceInfo, layout);
////	skybox = SkyBox(DeviceInfo, cubeTexture, renderer.skyBoxPipeline);
//
//	//modelLoader = ModelLoader(DeviceInfo, FileSystem::getPath("VulkanGraphics/Models/Nanosuit/nanosuit.obj"));
//
//	//std::vector<Texture2D> textureList = { texture, texture2 };
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//meshList.emplace_back(Mesh(DeviceInfo, meshvertices, indices, textureList));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//	//modelList.emplace_back(Model(DeviceInfo, modelLoader.GetModelMeshs()));
//
//	renderer.createCommandBuffers();
//	renderer.createSyncObjects();
//}
//
//VulkanGraphics::~VulkanGraphics()
//{
////	texture.Destroy();
////	texture2.Destroy();
////	cubeTexture.Destroy();
////	modelLoader.CleanTextureMemory();
//
//	renderer.ClearSwapChain();
//	renderer.Destory();
//
//	vkDestroyDevice(DeviceInfo.Device, nullptr);
//
//	//VulkanDebug.CleanUp(vulkanDevice2.Instance);
//
//	//vkDestroySurfaceKHR(vulkanDevice2.Instance, renderer.surface, nullptr);
//	//vkDestroyInstance(vulkanDevice2.Instance, nullptr);
//	//vulkanDevice2.Window.CleanUp();
//}
//
//void VulkanGraphics::mainLoop() {
//	//while (!glfwWindowShouldClose(vulkanDevice2.Window.GetWindowPtr()))
//	//{
//	//	vulkanDevice2.Window.Update();
//	//	drawFrame();
//	//	UpdateMouse();
//	//	UpdateKeyboard();
//	//}
//
//	vkDeviceWaitIdle(DeviceInfo.Device);
//}
//
//void VulkanGraphics::recreateSwapChain() {
//	int width = 0, height = 0;
//	//glfwGetFramebufferSize(vulkanDevice2.Window.GetWindowPtr(), &width, &height);
//	while (width == 0 || height == 0) {
//		//glfwGetFramebufferSize(vulkanDevice2.Window.GetWindowPtr(), &width, &height);
//		glfwWaitEvents();
//	}
//
//	vkDeviceWaitIdle(DeviceInfo.Device);
//
//	renderer.ClearSwapChain();
//	renderer.UpdateSwapChain();
//	//skybox.UpdateSwapChain(renderer.skyBoxPipeline);
//	//for (int x = 0; x < meshList.size(); x++)
//	//{
//	//	meshList[x].UpdateSwapChain();
//	//}
//	//for (int x = 0; x < modelList.size(); x++)
//	//{
//	//	modelList[x].UpdateSwapChain();
//	//}
//	renderer.createCommandBuffers();
//}
//
//void VulkanGraphics::updateUniformBuffer(uint32_t currentImage) {
//	static auto startTime = std::chrono::high_resolution_clock::now();
//
//	auto currentTime = std::chrono::high_resolution_clock::now();
//	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//	glm::vec3 cubePositions[] =
//	{
//		glm::vec3(0.0f,  0.0f,  0.0f),
//		glm::vec3(2.0f,  5.0f, -15.0f),
//		glm::vec3(-1.5f, -2.2f, -2.5f),
//		glm::vec3(-3.8f, -2.0f, -12.3f),
//		glm::vec3(2.4f, -0.4f, -3.5f),
//		glm::vec3(-1.7f,  3.0f, -7.5f),
//		glm::vec3(1.3f, -2.0f, -2.5f),
//		glm::vec3(1.5f,  2.0f, -2.5f),
//		glm::vec3(1.5f,  0.2f, -1.5f),
//		glm::vec3(-1.3f,  1.0f, -1.5f)
//	};
//
//
//	//for (int x = 0; x < meshList.size(); x++)
//	//{
//	//	UniformBufferObject ubo2 = {};
//	//	ubo2.model = glm::mat4(1.0f);
//	//	ubo2.model = glm::translate(ubo2.model, cubePositions[x]);
//	//	ubo2.model = glm::rotate(ubo2.model, glm::radians(x * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
//	//	ubo2.view = camera.GetViewMatrix();
//	//	ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), renderer.swapChainExtent.width / (float)renderer.swapChainExtent.height, 0.1f, 100.0f);
//	//	ubo2.proj[1][1] *= -1;
//
//	//	meshList[x].UpdateUniformBuffer(ubo2, currentImage);
//	//	modelList[x].UpdateUniformBuffer(ubo2, currentImage);
//	//}
//
//	//SkyBoxUniformBufferObject ubo = {};
//	//ubo.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
//	//ubo.projection = glm::perspective(glm::radians(camera.GetCameraZoom()), (float)renderer.swapChainExtent.width / (float)renderer.swapChainExtent.height, 0.1f, 100.0f);
//	//ubo.projection[1][1] *= -1;
//
//	//skybox.UpdateUniformBuffer(ubo, currentImage);
//}
//
//void VulkanGraphics::drawFrame() 
//{
//	uint32_t imageIndex;
//	VkSemaphore image_acquired_semaphore = renderer.imageAvailableSemaphores[currentFrame];
//	VkSemaphore render_complete_semaphore = renderer.renderFinishedSemaphores[currentFrame];
//	VkResult result = vkAcquireNextImageKHR(renderer.device, renderer.swapChain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &imageIndex);
//
//	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
//		recreateSwapChain();
//		return;
//	}
//	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
//		throw std::runtime_error("failed to acquire swap chain image!");
//	}
//
//	vkDeviceWaitIdle(DeviceInfo.Device);
//	vkWaitForFences(renderer.device, 1, &renderer.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
//	vkResetFences(renderer.device, 1, &renderer.inFlightFences[currentFrame]);
//	vkResetCommandPool(renderer.device, renderer.commandPool, 0);
//
//	renderer.UpdateFrame(MapX, MapY, SpriteX, SpriteY);
//
//	VkCommandBufferBeginInfo beginInfo = {};
//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//	beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//
//	if (vkBeginCommandBuffer(renderer.commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
//		throw std::runtime_error("failed to begin recording command buffer!");
//	}
//
//	std::array<VkClearValue, 2> clearValues = {};
//	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//	clearValues[1].depthStencil = { 1.0f, 0 };
//
//	VkRenderPassBeginInfo renderPassInfo = {};
//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//	renderPassInfo.renderPass = renderer.renderPass;
//	renderPassInfo.framebuffer = renderer.swapChainFramebuffers[currentFrame];
//	renderPassInfo.renderArea.offset = { 0, 0 };
//	renderPassInfo.renderArea.extent = renderer.swapChainExtent;
//	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//	renderPassInfo.pClearValues = clearValues.data();
//
//	vkCmdBeginRenderPass(renderer.commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//	renderer.DrawFrame(currentFrame);
//	vkCmdEndRenderPass(renderer.commandBuffers[currentFrame]);
//
//	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
//	VkSubmitInfo submitInfo = {};
//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//	submitInfo.waitSemaphoreCount = 1;
//	submitInfo.pWaitSemaphores = &image_acquired_semaphore;
//	submitInfo.pWaitDstStageMask = waitStages;
//	submitInfo.commandBufferCount = 1;
//	submitInfo.pCommandBuffers = &renderer.commandBuffers[imageIndex];
//	submitInfo.signalSemaphoreCount = 1;
//	submitInfo.pSignalSemaphores = &render_complete_semaphore;
//
//	if (vkEndCommandBuffer(renderer.commandBuffers[currentFrame]) != VK_SUCCESS) {
//		throw std::runtime_error("failed to record command buffer!");
//	}
//
//	if (vkQueueSubmit(renderer.graphicsQueue, 1, &submitInfo, renderer.inFlightFences[currentFrame]) != VK_SUCCESS) {
//		throw std::runtime_error("failed to submit draw command buffer!");
//	}
//
//	VkSwapchainKHR swapChains[] = { renderer.swapChain };
//
//	VkPresentInfoKHR presentInfo = {};
//	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//	presentInfo.waitSemaphoreCount = 1;
//	presentInfo.pWaitSemaphores = &render_complete_semaphore;
//	presentInfo.swapchainCount = 1;
//	presentInfo.pSwapchains = swapChains;
//	presentInfo.pImageIndices = &imageIndex;
//	result = vkQueuePresentKHR(renderer.presentQueue, &presentInfo);
//	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
//		recreateSwapChain();
//		return;
//	}
//	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
//		throw std::runtime_error("failed to acquire swap chain image!");
//	}
//
//	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//}
//
//bool VulkanGraphics::checkDeviceExtensionSupport(VkPhysicalDevice device) {
//	uint32_t extensionCount;
//	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//
//	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
//
//	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
//
//	for (const auto& extension : availableExtensions) {
//		requiredExtensions.erase(extension.extensionName);
//	}
//
//	return requiredExtensions.empty();
//}
//
//std::vector<const char*> VulkanGraphics::getRequiredExtensions() {
//	uint32_t glfwExtensionCount = 0;
//	const char** glfwExtensions;
//	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//	if (enableValidationLayers) {
//		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//	}
//
//	return extensions;
//}
//
//bool VulkanGraphics::checkValidationLayerSupport() {
//	uint32_t layerCount;
//	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//
//	std::vector<VkLayerProperties> availableLayers(layerCount);
//	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
//
//	for (const char* layerName : validationLayers) {
//		bool layerFound = false;
//
//		for (const auto& layerProperties : availableLayers) {
//			if (strcmp(layerName, layerProperties.layerName) == 0) {
//				layerFound = true;
//				break;
//			}
//		}
//
//		if (!layerFound) {
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void VulkanGraphics::UpdateMouse()
//{
//	//if (glfwGetMouseButton(vulkanDevice2.Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
//	//{
//	//	glfwGetCursorPos(vulkanDevice2.Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
//	//	if (firstMouse)
//	//	{
//	//		lastX = MouseXPos;
//	//		lastY = MouseYPos;
//	//		firstMouse = false;
//	//	}
//
//	//	float xoffset = MouseXPos - lastX;
//	//	float yoffset = lastY - MouseYPos;
//
//	//	lastX = MouseXPos;
//	//	lastY = MouseYPos;
//
//	//	camera.UpdateMouse(xoffset, yoffset);
//	//}
//	//else
//	//{
//	//	firstMouse = true;
//	//}
//}
//
//void VulkanGraphics::UpdateKeyboard()
//{
//	float currentFrame = glfwGetTime();
//	deltaTime = currentFrame - lastFrame;
//	lastFrame = currentFrame;
//
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
//	//{
//	//	camera.UpdateKeyboard(FORWARD, deltaTime);
//	//	MapY -= 25;
//	//}
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
//	//{
//	//	camera.UpdateKeyboard(BACKWARD, deltaTime);
//	//	MapY += 25;
//	//}
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
//	//{
//	//	camera.UpdateKeyboard(LEFT, deltaTime);
//	//	MapX -= 25;
//	//}
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
//	//{
//	//	camera.UpdateKeyboard(RIGHT, deltaTime);
//	//	MapX += 25;
//	//}
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_LEFT) == GLFW_PRESS)
//	//	SpriteX -= 25;
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_RIGHT) == GLFW_PRESS)
//	//	SpriteX += 25;
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_UP) == GLFW_PRESS)
//	//	SpriteY -= 25;
//	//if (glfwGetKey(vulkanDevice2.Window.GetWindowPtr(), GLFW_KEY_DOWN) == GLFW_PRESS)
//	//	SpriteY += 25;
//}
//
//void VulkanGraphics::VulkanRun()
//{
//	mainLoop();
//}
