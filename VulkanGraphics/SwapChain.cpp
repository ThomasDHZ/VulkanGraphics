#include "SwapChain.h"
#include <GLFW\glfw3.h>
#include <stdexcept>

SwapChain::SwapChain()
{
}

SwapChain::SwapChain(GLFWwindow* window, const VkInstance& instance, VkDevice device, VkPhysicalDevice PhysicalDevice)
{
	SetUpSwapChain(window, instance, device, PhysicalDevice);
}

SwapChain::~SwapChain()
{
}

VkSurfaceFormatKHR SwapChain::FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

void SwapChain::FindQueueFamilies(VkPhysicalDevice PhysicalDevice)
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
		vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, SwapChainSurface, &presentSupport);

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

void SwapChain::SetUpSwapChain(GLFWwindow* window, const VkInstance& instance, VkDevice device, VkPhysicalDevice PhysicalDevice)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &SwapChainSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, SwapChainSurface, &SwapChainCapabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, SwapChainSurface, &SurfaceFormatCount, nullptr);
	if (SurfaceFormatCount != 0) 
	{
		CompatibleSwapChainFormatList.resize(SurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, SwapChainSurface, &SurfaceFormatCount, CompatibleSwapChainFormatList.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, SwapChainSurface, &PresentModeCount, nullptr);
	if (PresentModeCount != 0)
	{
		CompatiblePresentModesList.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, SwapChainSurface, &PresentModeCount, CompatiblePresentModesList.data());
	}	

	FindQueueFamilies(PhysicalDevice);
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

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = SwapChainSurface;
	createInfo.minImageCount = SwapChainImageCount;
	createInfo.imageFormat = SwapChainFormat.format;
	createInfo.imageColorSpace = SwapChainFormat.colorSpace;
	createInfo.imageExtent = SwapChainSize;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = SwapChainCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = SwapChainPresentMode;
	createInfo.clipped = VK_TRUE;
	if(GraphicsFamily != PresentFamily) 
	{
		uint32_t queueFamilyIndices[] = { GraphicsFamily, PresentFamily };

		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &Swapchain) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	SwapChainImages.resize(SwapChainImageCount);
	vkGetSwapchainImagesKHR(device, Swapchain, &SwapChainImageCount, SwapChainImages.data());
}