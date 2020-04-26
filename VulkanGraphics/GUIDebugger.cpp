#include "GUIDebugger.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

GUIDebugger::GUIDebugger()
{
}

GUIDebugger::GUIDebugger(ImGui_ImplVulkan_InitInfo ImGUIStartUpInfo, GLFWwindow* window, VkRenderPass renderPass)
{
	ImGuiCommandBuffers.resize(ImGUIStartUpInfo.ImageCount);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = ImGUIStartUpInfo.QueueFamily;

	if (vkCreateCommandPool(ImGUIStartUpInfo.Device, &poolInfo, nullptr, &ImGuiCommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics command pool!");
	}

	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	if (vkCreateDescriptorPool(ImGUIStartUpInfo.Device, &pool_info, nullptr, &ImGuiDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	VkCommandBufferAllocateInfo allocInfo2{};
	allocInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo2.commandPool = ImGuiCommandPool;
	allocInfo2.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo2.commandBufferCount = (uint32_t)ImGuiCommandBuffers.size();

	if (vkAllocateCommandBuffers(ImGUIStartUpInfo.Device, &allocInfo2, ImGuiCommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGUIStartUpInfo.DescriptorPool = ImGuiDescriptorPool;
	ImGUIStartUpInfo.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&ImGUIStartUpInfo, renderPass);

	VkCommandBuffer command_buffer = VulkanBufferManager::beginSingleTimeCommands(ImGUIStartUpInfo.Device, ImGuiCommandPool);
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
	VulkanBufferManager::endSingleTimeCommands(ImGUIStartUpInfo.Device, command_buffer, ImGuiCommandPool, ImGUIStartUpInfo.Queue);
}

GUIDebugger::~GUIDebugger()
{
}

void GUIDebugger::UpdateCommandBuffers(uint32_t currentFrame, VkRenderPass renderPass, VkFramebuffer framebuffer)
{
	VkCommandBufferInheritanceInfo InheritanceInfo = {};
	InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	InheritanceInfo.renderPass = renderPass;
	InheritanceInfo.framebuffer = framebuffer;

	VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
	BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

	vkBeginCommandBuffer(ImGuiCommandBuffers[currentFrame], &BeginSecondaryCommandBuffer);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ImGuiCommandBuffers[currentFrame]);
	if (vkEndCommandBuffer(ImGuiCommandBuffers[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void GUIDebugger::UpdateGuiDebugger()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::Begin("Hello, world!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Render();
}

void GUIDebugger::ShutDown(VkDevice device)
{
	vkDestroyDescriptorPool(device, ImGuiDescriptorPool, nullptr);
	vkDestroyCommandPool(device, ImGuiCommandPool, nullptr);

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
