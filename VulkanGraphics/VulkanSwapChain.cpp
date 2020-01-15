#include "VulkanSwapChain.h"
#include <set>
#include <algorithm>

//SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR VulkanSurface)
//{
//	SwapChainSupportDetails Details;
//	unsigned int FormatCount;
//	unsigned int PresentModeCount;
//
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanSurface, &Details.capabilities);
//	vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, nullptr);
//
//	if (FormatCount != 0)
//	{
//		Details.formats.resize(FormatCount);
//		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, Details.formats.data());
//	}
//
//	vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());
//
//	if (PresentModeCount != 0)
//	{
//		Details.presentModes.resize(PresentModeCount);
//		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());
//	}
//
//	return Details;
//}
//
//VkSurfaceFormatKHR VulkanSwapChain::SetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
//{
//	for (const auto& availableFormat : AvailableFormats)
//	{
//		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
//		{
//			return availableFormat;
//		}
//	}
//	return AvailableFormats[0];
//}
//
//VkPresentModeKHR VulkanSwapChain::SetSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
//{
//	for (const auto& availablePresentMode : AvailablePresentModes)
//	{
//		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
//		{
//			return availablePresentMode;
//		}
//	}
//
//	return VK_PRESENT_MODE_FIFO_KHR;
//}
//
//VkExtent2D VulkanSwapChain::SetSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, VulkanWindow Window)
//{
//	if (Capabilities.currentExtent.width != UINT32_MAX) {
//		return Capabilities.currentExtent;
//	}
//	else
//	{
//		int width = Window.GetWindowWidth();
//		int height = Window.GetWindowHeight();
//
//		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
//		VkExtent2D ActualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
//		ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
//		ActualExtent.height = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));
//
//		return ActualExtent;
//	}
//}
