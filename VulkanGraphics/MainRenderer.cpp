#include "MainRenderer.h"

MainRenderer::MainRenderer()
{
}

MainRenderer::MainRenderer(VulkanRenderer& renderer)
{
	InitializeRenderPass(renderer);
	InitializeFramebuffers(renderer);
	//GraphicsPipeline = ForwardRenderingPipeline(renderer.SwapChain.GetSwapChainResolution(), RenderPass, renderer.Device);
}

MainRenderer::~MainRenderer()
{
}

void MainRenderer::InitializeRenderPass(VulkanRenderer& renderer)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	//depthAttachment.format = renderer.findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	//if (vkCreateRenderPass(renderer.Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create render pass!");
	//}
}

void MainRenderer::InitializeFramebuffers(VulkanRenderer& renderer)
{
	//DepthAttachment = InputAttachment(renderer.Device, renderer.PhysicalDevice, AttachmentType::VkDepthAttachemnt, renderer.SwapChain.GetSwapChainResolution().width, renderer.SwapChain.GetSwapChainResolution().height);

	//SwapChainFramebuffers.resize(renderer.SwapChain.GetSwapChainImageCount());

	//for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++) {
	//	std::array<VkImageView, 2> attachments =
	//	{
	//		renderer.SwapChain.GetSwapChainImageViews()[i],
	//		DepthAttachment.AttachmentImageView
	//	};

	//	VkFramebufferCreateInfo framebufferInfo{};
	//	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//	framebufferInfo.renderPass = RenderPass;
	//	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	//	framebufferInfo.pAttachments = attachments.data();
	//	framebufferInfo.width = renderer.SwapChain.GetSwapChainResolution().width;
	//	framebufferInfo.height = renderer.SwapChain.GetSwapChainResolution().height;
	//	framebufferInfo.layers = 1;

	//	if (vkCreateFramebuffer(renderer.Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to create framebuffer!");
	//	}
	//}
}