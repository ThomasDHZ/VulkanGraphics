#include "Renderer.h"
#include <set>

Renderer::Renderer() : RendererBase()
{
}

Renderer::Renderer(GLFWwindow* window) : RendererBase(window)
{
	SwapChain = VulkanSwapChain(window, Device, PhysicalDevice, Surface);
	InitializeRenderPass();
	InitializeFramebuffers();
	InitializeCommandBuffers();
	InitializeSyncObjects();
	InitializeGUIDebugger(window);

	GraphicsPipeline = ForwardRenderingPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	FrameBufferPipeline = FrameBufferRenderingPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	DebugLightPipeline = DebugLightRenderingPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	DebugCollisionPipeline = CollisionDebugPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	MeshviewPipeline = WireFramePipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	SkyboxPipeline = SkyBoxPipeline(SwapChain.GetSwapChainResolution(), RenderPass, Device);

	framebuffer = FrameBufferMesh(Device, PhysicalDevice, MainCommandPool, GraphicsQueue, SwapChain.GetSwapChainResolution(), RenderPass, HDRColorAttachment, DepthAttachment, FrameBufferPipeline.ShaderPipelineDescriptorLayout, SwapChain.GetSwapChainImageCount());
}

Renderer::~Renderer()
{
}

void Renderer::InitializeRenderPass()
{
	std::vector<VkAttachmentDescription> RenderPassAttachmentList;
	std::vector<VkSubpassDescription> SubpassDescriptionList;
	std::vector<VkSubpassDependency> SubpassDependencyList;

	VkAttachmentDescription SwapChainAttachment = {};
	SwapChainAttachment.format = SwapChain.GetSwapChainImageFormat().format;
	SwapChainAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	SwapChainAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	SwapChainAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	SwapChainAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	SwapChainAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	SwapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	SwapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription ColorAttachment = {};
	ColorAttachment.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentDescription DepthAttachment = {};
	DepthAttachment.format = findDepthFormat();
	DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthReference = { 2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkAttachmentReference colorReferenceSwapchain = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkAttachmentReference inputReferences[2];
	inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
	inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

	VkSubpassDescription FirstSubPassDescription = {};
	FirstSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	FirstSubPassDescription.colorAttachmentCount = 1;
	FirstSubPassDescription.pColorAttachments = &colorReference;
	FirstSubPassDescription.pDepthStencilAttachment = &depthReference;
	FirstSubPassDescription.inputAttachmentCount = 0;

	VkSubpassDescription SecondSubPassDescription = {};
	SecondSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	SecondSubPassDescription.colorAttachmentCount = 1;
	SecondSubPassDescription.pColorAttachments = &colorReferenceSwapchain;
	SecondSubPassDescription.inputAttachmentCount = 2;
	SecondSubPassDescription.pInputAttachments = inputReferences;


	VkSubpassDependency FirstDependency = {};
	FirstDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	FirstDependency.dstSubpass = 0;
	FirstDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	FirstDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	FirstDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	FirstDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	FirstDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency SecondDependency = {};
	SecondDependency.srcSubpass = 0;
	SecondDependency.dstSubpass = 1;
	SecondDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	SecondDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	SecondDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	SecondDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	SecondDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency ThirdDependency = {};
	ThirdDependency.srcSubpass = 0;
	ThirdDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	ThirdDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	ThirdDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	ThirdDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	ThirdDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	ThirdDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	RenderPassAttachmentList.emplace_back(SwapChainAttachment);
	RenderPassAttachmentList.emplace_back(ColorAttachment);
	RenderPassAttachmentList.emplace_back(DepthAttachment);

	SubpassDescriptionList.emplace_back(FirstSubPassDescription);
	SubpassDescriptionList.emplace_back(SecondSubPassDescription);

	SubpassDependencyList.emplace_back(FirstDependency);
	SubpassDependencyList.emplace_back(SecondDependency);
	SubpassDependencyList.emplace_back(ThirdDependency);

	VkRenderPassCreateInfo RenderPassInfo{};
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.attachmentCount = static_cast<uint32_t>(RenderPassAttachmentList.size());
	RenderPassInfo.pAttachments = RenderPassAttachmentList.data();
	RenderPassInfo.subpassCount = static_cast<uint32_t>(SubpassDescriptionList.size());
	RenderPassInfo.pSubpasses = SubpassDescriptionList.data();
	RenderPassInfo.dependencyCount = static_cast<uint32_t>(SubpassDependencyList.size());
	RenderPassInfo.pDependencies = SubpassDependencyList.data();

	if (vkCreateRenderPass(Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void Renderer::InitializeFramebuffers()
{
	HDRColorAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkHDRColorAttachment, SwapChain.GetSwapChainResolution().width, SwapChain.GetSwapChainResolution().height);
	DepthAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkDepthAttachemnt, SwapChain.GetSwapChainResolution().width, SwapChain.GetSwapChainResolution().height);

	SwapChainFramebuffers.resize(SwapChain.GetSwapChainImageCount());

	for (size_t i = 0; i < SwapChain.GetSwapChainImageCount(); i++) {
		std::array<VkImageView, 3> attachments =
		{
			SwapChain.GetSwapChainImageViews()[i],
			HDRColorAttachment.AttachmentImageView,
			DepthAttachment.AttachmentImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = SwapChain.GetSwapChainResolution().width;
		framebufferInfo.height = SwapChain.GetSwapChainResolution().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void Renderer::InitializeCommandBuffers()
{
	MainCommandBuffer.resize(SwapChain.GetSwapChainImageCount());
	SecondaryCommandBuffers.resize(SwapChain.GetSwapChainImageCount());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = GraphicsFamily;

	if (vkCreateCommandPool(Device, &poolInfo, nullptr, &MainCommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics command pool!");
	}

	if (vkCreateCommandPool(Device, &poolInfo, nullptr, &SecondaryCommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics command pool!");
	}

	VkCommandBufferAllocateInfo MainAllocInfo{};
	MainAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	MainAllocInfo.commandPool = MainCommandPool;
	MainAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	MainAllocInfo.commandBufferCount = (uint32_t)MainCommandBuffer.size();

	if (vkAllocateCommandBuffers(Device, &MainAllocInfo, MainCommandBuffer.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = SecondaryCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = (uint32_t)SecondaryCommandBuffers.size();

	if (vkAllocateCommandBuffers(Device, &allocInfo, SecondaryCommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void Renderer::InitializeSyncObjects()
{
	vulkanSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(SwapChain.GetSwapChainImageCount(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &vulkanSemaphores[i].ImageAcquiredSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &vulkanSemaphores[i].RenderCompleteSemaphore) != VK_SUCCESS ||
			vkCreateFence(Device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
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

	guiDebugger = GUIDebugger(init_info, window, RenderPass);
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

	for (auto framebuffer : SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(Device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(Device, SecondaryCommandPool, static_cast<uint32_t>(SecondaryCommandBuffers.size()), SecondaryCommandBuffers.data());

	for (auto imageView : SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(Device, imageView, nullptr);
	}

	vkDestroyCommandPool(Device, MainCommandPool, nullptr);
	vkDestroyCommandPool(Device, SecondaryCommandPool, nullptr);
	vkDestroySwapchainKHR(Device, SwapChain.GetSwapChain(), nullptr);

	SwapChain.UpdateSwapChain(window, Device, PhysicalDevice, Surface);
	GraphicsPipeline.UpdateSwapChain();
	DebugLightPipeline.UpdateSwapChain();
	DebugCollisionPipeline.UpdateSwapChain();
	FrameBufferPipeline.UpdateSwapChain();
	MeshviewPipeline.UpdateSwapChain();
	SkyboxPipeline.UpdateSwapChain();

	GraphicsPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	DebugLightPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	DebugCollisionPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	FrameBufferPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	MeshviewPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	SkyboxPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), RenderPass, Device);
	InitializeFramebuffers();
	InitializeCommandBuffers();

	framebuffer.RecreateSwapChainStage(Device, SwapChain.GetSwapChainResolution(), RenderPass, HDRColorAttachment, DepthAttachment, FrameBufferPipeline.ShaderPipelineDescriptorLayout, SwapChain.GetSwapChainImageCount());
	UpdateCommandBuffers = true;
}

void Renderer::Draw(GLFWwindow* window)
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

	RunCommandBuffers.clear();
	RunCommandBuffers.emplace_back(SecondaryCommandBuffers[DrawFrame]);
	//RunCommandBuffers.emplace_back(guiDebugger.GetCommandBuffers(DrawFrame));

	std::array<VkClearValue, 3> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clearValues[2].depthStencil = { 1.0f, 0 };

	if (imagesInFlight[DrawFrame] != VK_NULL_HANDLE) {
		vkWaitForFences(Device, 1, &imagesInFlight[DrawFrame], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[DrawFrame] = inFlightFences[currentFrame];

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = RenderPass;
	renderPassInfo.framebuffer = SwapChainFramebuffers[DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	VkCommandBufferBeginInfo CommandBufferInfo = {};
	CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	vkBeginCommandBuffer(MainCommandBuffer[DrawFrame], &CommandBufferInfo);
	vkCmdBeginRenderPass(MainCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	vkCmdExecuteCommands(MainCommandBuffer[DrawFrame], RunCommandBuffers.size(), RunCommandBuffers.data());
	vkCmdNextSubpass(MainCommandBuffer[DrawFrame], VK_SUBPASS_CONTENTS_INLINE);
	framebuffer.Draw(FrameBufferPipeline, MainCommandBuffer[DrawFrame], DrawFrame);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), MainCommandBuffer[DrawFrame]);
	vkCmdEndRenderPass(MainCommandBuffer[DrawFrame]);
	vkEndCommandBuffer(MainCommandBuffer[DrawFrame]);

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

	result = vkQueuePresentKHR(PresentQueue, &presentInfo);

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

	GraphicsPipeline.Destroy();
	DebugLightPipeline.Destroy();
	DebugCollisionPipeline.Destroy();
	FrameBufferPipeline.Destroy();
	MeshviewPipeline.Destroy();
	SkyboxPipeline.Destroy();
	
	framebuffer.Destory(Device);

	SwapChain.Destroy(Device);

	vkDestroyCommandPool(Device, MainCommandPool, nullptr);
	vkDestroyCommandPool(Device, SecondaryCommandPool, nullptr);

	vkDestroyRenderPass(Device, RenderPass, nullptr);

	for (auto& framebuffer : SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(Device, framebuffer, nullptr);
		framebuffer = VK_NULL_HANDLE;
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vulkanSemaphores[i].Destory(Device);
		vkDestroyFence(Device, inFlightFences[i], nullptr);

		inFlightFences[i] = VK_NULL_HANDLE;
	}

	vkDestroyDevice(Device, nullptr);

	MainCommandPool = VK_NULL_HANDLE;
	SecondaryCommandPool = VK_NULL_HANDLE;
	RenderPass = VK_NULL_HANDLE;

	RendererBase::Destory();
}
