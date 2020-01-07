#include "ValkanGraphics.h"

ValkanGraphics::ValkanGraphics(unsigned int width, unsigned int height, const char* windowName) : EnableValidationLayers(true)
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

	Window = VulkanWindow(width, height, windowName);

	SetUpVulkanInstance();
	SetUpDebugger();
	SetUpPhysicalDevice();
	SetUpLogicalDevice();
}

ValkanGraphics::~ValkanGraphics()
{
	VulkanDebug.CleanUp(VulkanInstance);
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
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		
		if (FindQueueFamilies(device).IsComplete() &&
			deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			deviceFeatures.geometryShader)
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
	VulkanQueueFamily Indices = FindQueueFamilies(GPUInfo.PhysicalDevice);
	float QueuePriority = 1.0f;

	VkDeviceQueueCreateInfo QueueCreateInfo = {};
	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.queueFamilyIndex = Indices.GraphicsFamily.value();
	QueueCreateInfo.queueCount = 1;
	QueueCreateInfo.pQueuePriorities = &QueuePriority;

	VkPhysicalDeviceFeatures DeviceFeatures = {};

	VkDeviceCreateInfo CreateDeviceInfo = {};
	CreateDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateDeviceInfo.pQueueCreateInfos = &QueueCreateInfo;
	CreateDeviceInfo.queueCreateInfoCount = 1;
	CreateDeviceInfo.pEnabledFeatures = &DeviceFeatures;
	CreateDeviceInfo.enabledExtensionCount = 0;
	
	if (EnableValidationLayers)
	{
		CreateDeviceInfo.enabledLayerCount = static_cast<unsigned int>(ValidationLayers.size());
		CreateDeviceInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		CreateDeviceInfo.enabledLayerCount = 0;
	}

	VkResult Result = vkCreateDevice(GPUInfo.PhysicalDevice, &CreateDeviceInfo, nullptr, &GPUInfo.Device);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device.");
	}
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
		if (QueueFamilies[x].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			Indices.GraphicsFamily = x;
		}

		if (Indices.IsComplete()) 
		{
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

