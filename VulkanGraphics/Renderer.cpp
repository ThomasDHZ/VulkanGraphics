#include "Renderer.h"
#include <set>

Renderer::Renderer() : RendererBase()
{
}

Renderer::Renderer(GLFWwindow* window) : RendererBase(window)
{
	forwardRenderer = ForwardRenderer(Device, PhysicalDevice, SwapChain.GetSwapChainResolution(), SwapChain.GetSwapChainImageViews());

	InitializeOffscreenRenderPass();
	InitializeOffscreenFramebuffers();
	InitializeGUIDebugger(window);

	//renderToTexturePipeline = RenderToTexturePipeline(SwapChain.GetSwapChainResolution(), OffscreenRenderPass, Device);
	//FrameBufferPipeline = FrameBufferRenderingPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//DebugLightPipeline = DebugLightRenderingPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//DebugCollisionPipeline = CollisionDebugPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//MeshviewPipeline = WireFramePipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//SkyboxPipeline = SkyBoxPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
}

Renderer::~Renderer()
{
}


void Renderer::InitializeOffscreenRenderPass()
{
	std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};

	attchmentDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	attchmentDescriptions[1].format = findDepthFormat();
	attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

	VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;

	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
	renderPassInfo.pAttachments = attchmentDescriptions.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(Device, &renderPassInfo, nullptr, &OffscreenRenderPass))
	{
		throw std::runtime_error("failed to create vkCreateImageView!");
	}
}

void Renderer::InitializeOffscreenFramebuffers()
{
	OffscreenHDRColorAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkHDRColorAttachment, SwapChain.GetSwapChainResolution().width, SwapChain.GetSwapChainResolution().height);
	OffscreenDepthAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkDepthAttachemnt, SwapChain.GetSwapChainResolution().width, SwapChain.GetSwapChainResolution().height);

	OffscreenSwapChainFramebuffers.resize(SwapChain.GetSwapChainImageCount());

	for (size_t i = 0; i < SwapChain.GetSwapChainImageCount(); i++) {
		std::array<VkImageView, 2> attachments =
		{
			OffscreenHDRColorAttachment.AttachmentImageView,
			OffscreenDepthAttachment.AttachmentImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = OffscreenRenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = SwapChain.GetSwapChainResolution().width;
		framebufferInfo.height = SwapChain.GetSwapChainResolution().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Device, &framebufferInfo, nullptr, &OffscreenSwapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void Renderer::InitializeGUIDebugger(GLFWwindow* window)
{
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = Instance;
	init_info.PhysicalDevice = PhysicalDevice;
	init_info.Device = Device;
	init_info.QueueFamily = 0;
	init_info.Queue = GraphicsQueue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = SwapChain.GetSwapChainMinImageCount();
	init_info.ImageCount = SwapChain.GetSwapChainImageCount();

	guiDebugger = GUIDebugger(init_info, window, forwardRenderer.RenderPass);
}

void Renderer::UpdateSwapChain(GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(Device);

	HDRColorAttachment.DeleteInputAttachment(Device);
	DepthAttachment.DeleteInputAttachment(Device);

	//for (auto framebuffer : SwapChainFramebuffers)
	//{
	//	vkDestroyFramebuffer(Device, framebuffer, nullptr);
	//}

	vkFreeCommandBuffers(Device, SecondaryCommandPool, static_cast<uint32_t>(SecondaryCommandBuffers.size()), SecondaryCommandBuffers.data());

	for (auto imageView : SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(Device, imageView, nullptr);
	}

	vkDestroyCommandPool(Device, MainCommandPool, nullptr);
	vkDestroyCommandPool(Device, SecondaryCommandPool, nullptr);
	vkDestroySwapchainKHR(Device, SwapChain.GetSwapChain(), nullptr);

	//SwapChain.UpdateSwapChain(window, Device, PhysicalDevice, Surface);
	//GraphicsPipeline.UpdateSwapChain();
	//renderToTexturePipeline.UpdateSwapChain();
	//DebugLightPipeline.UpdateSwapChain();
	//DebugCollisionPipeline.UpdateSwapChain();
	//FrameBufferPipeline.UpdateSwapChain();
	//MeshviewPipeline.UpdateSwapChain();
	//SkyboxPipeline.UpdateSwapChain();

	//GraphicsPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//renderToTexturePipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), OffscreenRenderPass, Device);
	//DebugLightPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//DebugCollisionPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//FrameBufferPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//MeshviewPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//SkyboxPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	//InitializeFramebuffers();
	//InitializeCommandBuffers();

	UpdateCommandBuffers = true;
}

uint32_t Renderer::DrawStart(GLFWwindow* window)
{
	vkWaitForFences(Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(Device, SwapChain.GetSwapChain(), UINT64_MAX, vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &DrawFrame);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		UpdateSwapChain(window);
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	if (imagesInFlight[DrawFrame] != VK_NULL_HANDLE) {
		vkWaitForFences(Device, 1, &imagesInFlight[DrawFrame], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[DrawFrame] = inFlightFences[currentFrame];

	return DrawFrame;
}

void Renderer::DrawEnd(GLFWwindow* window, uint32_t drawFrame)
{
	VkSemaphore waitSemaphores[] = { vulkanSemaphores[currentFrame].ImageAcquiredSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { vulkanSemaphores[currentFrame].RenderCompleteSemaphore };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &MainCommandBuffer[DrawFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(Device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(GraphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkSwapchainKHR swapChains[] = { SwapChain.GetSwapChain() };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &DrawFrame;

	VkResult result = vkQueuePresentKHR(PresentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		framebufferResized = false;
		UpdateSwapChain(window);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::DestoryVulkan()
{
	guiDebugger.ShutDown(Device);

	HDRColorAttachment.DeleteInputAttachment(Device);
	DepthAttachment.DeleteInputAttachment(Device);

	//GraphicsPipeline.Destroy();
	//renderToTexturePipeline.Destroy();
	//DebugLightPipeline.Destroy();
	//DebugCollisionPipeline.Destroy();
	//FrameBufferPipeline.Destroy();
	//MeshviewPipeline.Destroy();
	//SkyboxPipeline.Destroy();

	//SwapChain.Destroy(Device);

	//vkDestroyCommandPool(Device, MainCommandPool, nullptr);
	//vkDestroyCommandPool(Device, SecondaryCommandPool, nullptr);

	//vkDestroyRenderPass(Device, RenderPass, nullptr);

	//for (auto& framebuffer : SwapChainFramebuffers)
	//{
	//	vkDestroyFramebuffer(Device, framebuffer, nullptr);
	//	framebuffer = VK_NULL_HANDLE;
	//}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vulkanSemaphores[i].Destory(Device);
		vkDestroyFence(Device, inFlightFences[i], nullptr);

		inFlightFences[i] = VK_NULL_HANDLE;
	}

	vkDestroyDevice(Device, nullptr);

	MainCommandPool = VK_NULL_HANDLE;
	SecondaryCommandPool = VK_NULL_HANDLE;
	//RenderPass = VK_NULL_HANDLE;

	RendererBase::Destory();
}
