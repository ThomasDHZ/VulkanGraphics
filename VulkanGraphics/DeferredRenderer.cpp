//#include "DeferredRenderer.h"
//#include <vulkan\vulkan_core.h>
//#include <stdexcept>
//
//DeferredRenderer::DeferredRenderer()
//{
//}
//
//DeferredRenderer::~DeferredRenderer()
//{
//}
//
//void DeferredRenderer::createRenderPass() 
//{
//	std::array<VkAttachmentDescription, 5> attachments{};
//
//	attachments[0].format = swapChainImageFormat;
//	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
//	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//	attachments[1].format = VK_FORMAT_R16G16B16A16_SFLOAT;
//	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
//	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	attachments[1].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	attachments[2].format = VK_FORMAT_R16G16B16A16_SFLOAT;
//	attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
//	attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	attachments[2].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	attachments[3].format = VK_FORMAT_R8G8B8A8_UNORM;
//	attachments[3].samples = VK_SAMPLE_COUNT_1_BIT;
//	attachments[3].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	attachments[3].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[3].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	attachments[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[3].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	attachments[3].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	attachments[4].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
//	attachments[4].samples = VK_SAMPLE_COUNT_1_BIT;
//	attachments[4].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	attachments[4].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[4].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	attachments[4].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	attachments[4].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	attachments[4].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//	std::array<VkSubpassDescription, 2> subpassDescriptions{};
//
//	VkAttachmentReference colorReferences[4];
//	colorReferences[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[1] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[2] = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[3] = { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	VkAttachmentReference depthReference = { 4, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
//
//	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	subpassDescriptions[0].colorAttachmentCount = 4;
//	subpassDescriptions[0].pColorAttachments = colorReferences;
//	subpassDescriptions[0].pDepthStencilAttachment = &depthReference;
//
//
//	VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//
//	VkAttachmentReference inputReferences[3];
//	inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//	inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//	inputReferences[2] = { 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//
//	uint32_t preserveAttachmentIndex = 1;
//
//	subpassDescriptions[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	subpassDescriptions[1].colorAttachmentCount = 1;
//	subpassDescriptions[1].pColorAttachments = &colorReference;
//	subpassDescriptions[1].pDepthStencilAttachment = &depthReference;
//	subpassDescriptions[1].inputAttachmentCount = 3;
//	subpassDescriptions[1].pInputAttachments = inputReferences;
//
//
//	std::array<VkSubpassDependency, 3> dependencies;
//
//	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//	dependencies[0].dstSubpass = 0;
//	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	dependencies[1].srcSubpass = 0;
//	dependencies[1].dstSubpass = 1;
//	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	dependencies[2].srcSubpass = 0;
//	dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
//	dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//	dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//	dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	VkRenderPassCreateInfo renderPassInfo = {};
//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
//	renderPassInfo.pAttachments = attachments.data();
//	renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
//	renderPassInfo.pSubpasses = subpassDescriptions.data();
//	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
//	renderPassInfo.pDependencies = dependencies.data();
//
//	if (vkCreateRenderPass(DeviceInfo.Device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
//		throw std::runtime_error("failed to create render pass!");
//	}
//}
//
//void DeferredRenderer::createCommandBuffers()
//{
//	commandBuffers.resize(swapChainFramebuffers.size());
//
//	VkCommandBufferAllocateInfo allocInfo = {};
//	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//	allocInfo.commandPool = DeviceInfo.CommandPool;
//	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
//
//	if (vkAllocateCommandBuffers(DeviceInfo.Device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
//		throw std::runtime_error("failed to allocate command buffers!");
//	}
//
//	for (size_t i = 0; i < commandBuffers.size(); i++) {
//		VkCommandBufferBeginInfo beginInfo = {};
//		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//
//		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
//			throw std::runtime_error("failed to begin recording command buffer!");
//		}
//
//		VkClearValue clearValues[5];
//		clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
//		clearValues[1].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
//		clearValues[2].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
//		clearValues[3].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
//		clearValues[4].depthStencil = { 1.0f, 0 };
//
//		VkRenderPassBeginInfo renderPassInfo = {};
//		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//		renderPassInfo.renderPass = renderPass;
//		renderPassInfo.framebuffer = swapChainFramebuffers[i];
//		renderPassInfo.renderArea.offset.x = 0;
//		renderPassInfo.renderArea.offset.y = 0;
//		renderPassInfo.renderArea.extent.width = swapChainExtent.width;
//		renderPassInfo.renderArea.extent.height = swapChainExtent.height;
//		renderPassInfo.clearValueCount = 5;
//		renderPassInfo.pClearValues = clearValues;
//
//		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//}