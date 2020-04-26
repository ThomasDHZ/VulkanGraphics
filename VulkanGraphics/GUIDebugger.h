#pragma once
#include <vector>
#include <vulkan\vulkan_core.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

class GUIDebugger
{
private:
	VkDescriptorPool ImGuiDescriptorPool;
	VkCommandPool ImGuiCommandPool;
	std::vector<VkCommandBuffer> ImGuiCommandBuffers;

	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		printf("VkResult %d\n", err);
		if (err < 0)
			abort();
	}

public:
	GUIDebugger();
	GUIDebugger(ImGui_ImplVulkan_InitInfo ImGUIStartUpInfo, GLFWwindow* window, VkRenderPass renderPass);
	~GUIDebugger();

	void UpdateCommandBuffers(uint32_t currentFrame, VkRenderPass renderPass, VkFramebuffer framebuffer);
	void ShutDown(VkDevice device);

	VkCommandBuffer GetCommandBuffers(int index) { return ImGuiCommandBuffers[index]; }
};

