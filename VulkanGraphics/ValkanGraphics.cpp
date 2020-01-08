#include "ValkanGraphics.h"
#include <set>
#include <algorithm> 

ValkanGraphics::ValkanGraphics(unsigned int width, unsigned int height, const char* windowName) : EnableValidationLayers(true)
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
	DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	Window = VulkanWindow(width, height, windowName);

	SetUpVulkanInstance();
	SetUpDebugger();
	SetUpSurface();
	SetUpPhysicalDevice();
	SetUpLogicalDevice();
	SetUpSwapChain();
}

ValkanGraphics::~ValkanGraphics()
{
	vkDestroySwapchainKHR(GPUInfo.Device, SwapChain, nullptr);
	vkDestroyDevice(GPUInfo.Device, nullptr);
	VulkanDebug.CleanUp(VulkanInstance);
	vkDestroySurfaceKHR(VulkanInstance, VulkanSurface, nullptr);
	vkDestroyInstance(VulkanInstance, nullptr);
	Window.CleanUp();
}

void ValkanGraphics::SetUpVulkanInstance()
{
	if (EnableValidationLayers && !ValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available.");
	}

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtenstion;

	glfwExtenstion = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkApplicationInfo VKInfo = {};
	VKInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VKInfo.pApplicationName = "Valkan Graphics Testing";
	VKInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VKInfo.pEngineName = "No Engine";
	VKInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VKInfo.apiVersion = VK_API_VERSION_1_1;

	auto extensions = GetRequiredExtensions();

	VkInstanceCreateInfo CreateVKInfo = {};
	CreateVKInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateVKInfo.pApplicationInfo = &VKInfo;
	CreateVKInfo.enabledExtensionCount = glfwExtensionCount;
	CreateVKInfo.ppEnabledExtensionNames = glfwExtenstion;
	CreateVKInfo.enabledExtensionCount = static_cast<unsigned int>(extensions.size());
	CreateVKInfo.ppEnabledExtensionNames = extensions.data();


	if (EnableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

		CreateVKInfo.enabledLayerCount = static_cast<unsigned int>(ValidationLayers.size());
		CreateVKInfo.ppEnabledLayerNames = ValidationLayers.data();
		CreateVKInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}
	else
	{
		CreateVKInfo.enabledLayerCount = 0;
	}

	VkResult Result = vkCreateInstance(&CreateVKInfo, nullptr, &VulkanInstance);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan instance. Error:" + Result);
	}
}

void ValkanGraphics::SetUpDebugger()
{
	if (!EnableValidationLayers)
	{
		return;
	}

	VulkanDebug.SetUpDebugger(VulkanInstance);
}

void ValkanGraphics::SetUpPhysicalDevice()
{
	unsigned int GPUCount;
	vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, nullptr);

	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> GPUDevices(GPUCount);
	vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, GPUDevices.data());

	for (const auto& device : GPUDevices)
	{
		if (IsDeviceCompatable(device))
		{
			GPUInfo.PhysicalDevice = device;
		}

		if (GPUInfo.PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}
}

void ValkanGraphics::SetUpLogicalDevice()
{
	VulkanQueueFamily indices = FindQueueFamilies(GPUInfo.PhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

	if (EnableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		createInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(GPUInfo.PhysicalDevice, &createInfo, nullptr, &GPUInfo.Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(GPUInfo.Device, indices.GraphicsFamily.value(), 0, &GraphicsQueue);
	vkGetDeviceQueue(GPUInfo.Device, indices.PresentationFamily.value(), 0, &PresentQueue);
}

void ValkanGraphics::SetUpSurface()
{
	VkResult Result = glfwCreateWindowSurface(VulkanInstance, Window.GetWindowPtr(), nullptr, &VulkanSurface);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface.");
	}
}

void ValkanGraphics::SetUpSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(GPUInfo.PhysicalDevice);
	VkSurfaceFormatKHR surfaceFormat = SetSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = SetSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = SetSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = VulkanSurface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VulkanQueueFamily indices = FindQueueFamilies(GPUInfo.PhysicalDevice);
	uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

	if (indices.GraphicsFamily != indices.PresentationFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	try
	{
		if (vkCreateSwapchainKHR(GPUInfo.Device, &createInfo, nullptr, &SwapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	vkGetSwapchainImagesKHR(GPUInfo.Device, SwapChain, &imageCount, nullptr);
	SwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(GPUInfo.Device, SwapChain, &imageCount, SwapChainImages.data());

	SwapChainImageFormat = surfaceFormat.format;
	SwapChainExtent = extent;
}

VulkanQueueFamily ValkanGraphics::FindQueueFamilies(VkPhysicalDevice physicalDevice)
{
	VulkanQueueFamily Indices;

	unsigned int QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, QueueFamilies.data());

	for (int x = 0; x <= QueueFamilies.size() - 1; x++)
	{
		if (QueueFamilies[x].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			Indices.GraphicsFamily = x;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, x, VulkanSurface, &presentSupport);

		if (presentSupport) 
		{
			Indices.PresentationFamily = x;
		}

		if (Indices.IsComplete()) {
			break;
		}
	}

	return Indices;
}

bool ValkanGraphics::ValidationLayerSupport()
{
	unsigned int LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

	std::vector<VkLayerProperties> AvailableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	for (const char* LayerName : ValidationLayers)
	{
		bool LayerFound = false;

		for (const auto& LayerProperties : AvailableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				LayerFound = true;
				break;
			}
		}

		if (!LayerFound)
		{
			return false;
		}
	}

	return true;
}

bool ValkanGraphics::IsDeviceCompatable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties DeviceProperties;
	VkPhysicalDeviceFeatures DeviceFeatures;

	vkGetPhysicalDeviceProperties(device, &DeviceProperties);
	vkGetPhysicalDeviceFeatures(device, &DeviceFeatures);

	VulkanQueueFamily Indices = FindQueueFamilies(device);

	bool ExtensionSupported = CheckDeviceExtensionSupport(device);

	bool SwapChainAdequate = false;
	if (ExtensionSupported) 
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		SwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return Indices.IsComplete() && 
		   ExtensionSupported &&
		   SwapChainAdequate &&
		   DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		   DeviceFeatures.geometryShader;
}

bool ValkanGraphics::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	unsigned int ExtensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, nullptr);

	std::vector<VkExtensionProperties> AvailableExtension(ExtensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, AvailableExtension.data());

	std::set<std::string> RequiredExtension(DeviceExtensions.begin(), DeviceExtensions.end());

	for (const auto& extension : AvailableExtension)
	{
		RequiredExtension.erase(extension.extensionName);
	}

	return RequiredExtension.empty();
}

SwapChainSupportDetails ValkanGraphics::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails Details;
	unsigned int FormatCount;
	unsigned int PresentModeCount;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanSurface, &Details.capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, Details.formats.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());

	if (PresentModeCount != 0)
	{
		Details.presentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());
	}

	return Details;
}

VkSurfaceFormatKHR ValkanGraphics::SetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	for (const auto& availableFormat : AvailableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return AvailableFormats[0];
}

VkPresentModeKHR ValkanGraphics::SetSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
{
	for (const auto& availablePresentMode : AvailablePresentModes) 
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ValkanGraphics::SetSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{
	if (Capabilities.currentExtent.width != UINT32_MAX) {
		return Capabilities.currentExtent;
	}
	else 
	{
		VkExtent2D ActualExtent = { Window.GetWindowWidth(), Window.GetWindowHeight() };
		ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
		ActualExtent.height = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

		return ActualExtent;
	}
}

std::vector<const char*> ValkanGraphics::GetRequiredExtensions()
{
	unsigned int glfwExtenstionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtenstionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtenstionCount);

	if (EnableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void ValkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
	}
}

void ValkanGraphics::Run()
{
	MainLoop();
}

