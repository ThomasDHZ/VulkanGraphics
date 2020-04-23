#include "VulkanSwapChain.h"
#include <GLFW\glfw3.h>
#include <stdexcept>
#include <set>

VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::VulkanSwapChain(GLFWwindow* window)
{
	VulkanStart(window);
	SetUpSwapChain(window);
	SetUpSwapChainImageViews();
}

VulkanSwapChain::~VulkanSwapChain()
{
}

std::vector<const char*> VulkanSwapChain::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

bool VulkanSwapChain::checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
bool VulkanSwapChain::isDeviceSuitable(VkPhysicalDevice GPUDevice)
{
	FindQueueFamilies(GPUDevice, Surface);

	bool extensionsSupported = checkDeviceExtensionSupport(GPUDevice);


	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(GPUDevice, &supportedFeatures);

	uint32_t GPUSurfaceFormatCount;
	uint32_t GPUPresentModeCount;
	std::vector<VkSurfaceFormatKHR> GPUSwapChainFormatCapabilities;
	std::vector<VkPresentModeKHR> GPUPresentModesList;

	vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, nullptr);
	if (GPUSurfaceFormatCount != 0)
	{
		GPUSwapChainFormatCapabilities.resize(GPUSurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, GPUSwapChainFormatCapabilities.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, nullptr);
	if (GPUPresentModeCount != 0)
	{
		GPUPresentModesList.resize(GPUPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, GPUPresentModesList.data());
	}


	return GraphicsFamily != -1 && 
		   PresentFamily != -1 && 
		   extensionsSupported && 
		   GPUSwapChainFormatCapabilities.size() != 0 &&
		   GPUPresentModesList.size() != 0 &&
		   supportedFeatures.samplerAnisotropy;
}

VkSurfaceFormatKHR VulkanSwapChain::FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
{
	for (const auto& availablePresentMode : availablePresentModes) 
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

void VulkanSwapChain::FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			GraphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, Surface, &presentSupport);

		if (presentSupport) {
			PresentFamily = i;
		}

		if (GraphicsFamily != -1 &&
			PresentFamily != -1) 
		{
			break;
		}

		i++;
	}
}

void VulkanSwapChain::VulkanStart(GLFWwindow* window)
{
	bool DebuggerAvalible = false;
	uint32_t LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	VulkanLayers.resize(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, VulkanLayers.data());

	for (const char* LayerName : validationLayers)
	{
		if (LayerName == "VK_LAYER_KHRONOS_validation")
		{
			DebuggerAvalible = true;
			break;
		}
	}

	auto ExtensionList = getRequiredExtensions();

	VkApplicationInfo VulkanInfo = {};
	VulkanInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanInfo.pApplicationName = "Vulkan Graphics";
	VulkanInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanInfo.pEngineName = "No Engine";
	VulkanInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo VulkanCreateInfo = {};
	VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	VulkanCreateInfo.pApplicationInfo = &VulkanInfo;
	VulkanCreateInfo.enabledExtensionCount = static_cast<uint32_t>(ExtensionList.size());
	VulkanCreateInfo.ppEnabledExtensionNames = ExtensionList.data();

	if (DebuggerAvalible)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

		VulkanCreateInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
		VulkanCreateInfo.ppEnabledLayerNames = validationLayers.data();
		VulkanCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}
	else
	{
		VulkanCreateInfo.enabledLayerCount = 0;
		VulkanCreateInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&VulkanCreateInfo, nullptr, &Instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	VulkanDebug.SetUpDebugger(Instance);

	if (glfwCreateWindowSurface(Instance, window, nullptr, &Surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

	for (const auto& gpudevice : devices) {
		if (isDeviceSuitable(gpudevice))
		{
			PhysicalDevice = gpudevice;
			break;
		}
	}

	if (PhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { static_cast<uint32_t>(GraphicsFamily), static_cast<uint32_t>(PresentFamily) };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (DebuggerAvalible)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(Device, GraphicsFamily, 0, &GraphicsQueue);
	vkGetDeviceQueue(Device, PresentFamily, 0, &PresentQueue);
}

void VulkanSwapChain::SetUpSwapChain(GLFWwindow* window)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &SwapChainCapabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &SurfaceFormatCount, nullptr);
	if (SurfaceFormatCount != 0) 
	{
		CompatibleSwapChainFormatList.resize(SurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &SurfaceFormatCount, CompatibleSwapChainFormatList.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, nullptr);
	if (PresentModeCount != 0)
	{
		CompatiblePresentModesList.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, CompatiblePresentModesList.data());
	}	

	FindQueueFamilies(PhysicalDevice, Surface);
	SwapChainFormat = FindSwapSurfaceFormat(CompatibleSwapChainFormatList);
	SwapChainPresentMode = FindSwapPresentMode(CompatiblePresentModesList);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	SwapChainSize = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

	SwapChainImageCount = SwapChainCapabilities.minImageCount + 1;
	if (SwapChainCapabilities.maxImageCount > 0 &&
		SwapChainImageCount > SwapChainCapabilities.maxImageCount)
	{
		SwapChainImageCount = SwapChainCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR SwapChainCreateInfo = {};
	SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainCreateInfo.surface = Surface;
	SwapChainCreateInfo.minImageCount = SwapChainImageCount;
	SwapChainCreateInfo.imageFormat = SwapChainFormat.format;
	SwapChainCreateInfo.imageColorSpace = SwapChainFormat.colorSpace;
	SwapChainCreateInfo.imageExtent = SwapChainSize;
	SwapChainCreateInfo.imageArrayLayers = 1;
	SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	SwapChainCreateInfo.preTransform = SwapChainCapabilities.currentTransform;
	SwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainCreateInfo.presentMode = SwapChainPresentMode;
	SwapChainCreateInfo.clipped = VK_TRUE;
	if(GraphicsFamily != PresentFamily) 
	{
		uint32_t queueFamilyIndices[] = { GraphicsFamily, PresentFamily };

		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		SwapChainCreateInfo.queueFamilyIndexCount = 2;
		SwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	if (vkCreateSwapchainKHR(Device, &SwapChainCreateInfo, nullptr, &Swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(Device, Swapchain, &SwapChainImageCount, nullptr);
	SwapChainImages.resize(SwapChainImageCount);
	vkGetSwapchainImagesKHR(Device, Swapchain, &SwapChainImageCount, SwapChainImages.data());
}

void VulkanSwapChain::SetUpSwapChainImageViews()
{
	SwapChainImageViews.resize(SwapChainImageCount);
	for (uint32_t x = 0; x < SwapChainImageCount; x++)
	{
		VkImageViewCreateInfo SwapChainViewInfo = {};
		SwapChainViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		SwapChainViewInfo.image = SwapChainImages[x];
		SwapChainViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		SwapChainViewInfo.format = SwapChainFormat.format;
		SwapChainViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		SwapChainViewInfo.subresourceRange.baseMipLevel = 0;
		SwapChainViewInfo.subresourceRange.levelCount = 1;
		SwapChainViewInfo.subresourceRange.baseArrayLayer = 0;
		SwapChainViewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(Device, &SwapChainViewInfo, nullptr, &SwapChainImageViews[x]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture image view!");
		}
	}
}

void VulkanSwapChain::DestroyVulkan()
{
	vkDestroyDevice(Device, nullptr);
	VulkanDebug.CleanUp(Instance);
	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroyInstance(Instance, nullptr);
}
