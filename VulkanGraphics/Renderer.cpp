#include "Renderer.h"
#include <set>

Renderer::Renderer() : VulkanRenderer()
{
}

Renderer::Renderer(GLFWwindow* window) : VulkanRenderer(window)
{
	forwardRenderer = ForwardRenderer(*GetVulkanRendererBase());
	textureRenderer = TextureRenderer(*GetVulkanRendererBase());
	InitializeGUIDebugger(window);
}

Renderer::~Renderer()
{
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

	for (auto imageView : SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(Device, imageView, nullptr);
	}

	vkDestroyCommandPool(Device, RenderCommandPool, nullptr);
	vkDestroySwapchainKHR(Device, SwapChain.GetSwapChain(), nullptr);

	SwapChain.UpdateSwapChain(window, Device, PhysicalDevice, Surface);

	//DebugLightPipeline.UpdateSwapChain();
	//DebugCollisionPipeline.UpdateSwapChain();
	//MeshviewPipeline.UpdateSwapChain();
	//SkyboxPipeline.UpdateSwapChain();

	//DebugLightPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	//DebugCollisionPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	//MeshviewPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	//SkyboxPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	{

		forwardRenderer.DepthTexture.Delete(*GetVulkanRendererBase());

		for (auto framebuffer : forwardRenderer.SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(Device, framebuffer, nullptr);
		}

		vkDestroyPipeline(Device, forwardRenderer.RendererPipeline, nullptr);
		vkDestroyPipelineLayout(Device, forwardRenderer.RendererLayout, nullptr);

		forwardRenderer.RendererPipeline = VK_NULL_HANDLE;
		forwardRenderer.RendererLayout = VK_NULL_HANDLE;

		forwardRenderer.UpdateSwapChain(*GetVulkanRendererBase());
	}
	{

		textureRenderer.DepthTexture.Delete(*GetVulkanRendererBase());
		textureRenderer.ColorTexture.Delete(*GetVulkanRendererBase());
		for (auto framebuffer : textureRenderer.SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(Device, framebuffer, nullptr);
		}

		vkDestroyPipeline(Device, textureRenderer.RendererPipeline, nullptr);
		vkDestroyPipelineLayout(Device, textureRenderer.RendererLayout, nullptr);

		textureRenderer.RendererPipeline = VK_NULL_HANDLE;
		textureRenderer.RendererLayout = VK_NULL_HANDLE;


		textureRenderer.DepthTexture = RendererDepthTexture(*GetVulkanRendererBase());
		textureRenderer.ColorTexture = RendererColorTexture(*GetVulkanRendererBase());
		textureRenderer.CreateRenderingPipeline(*GetVulkanRendererBase());
		textureRenderer.CreateRendererFramebuffers(*GetVulkanRendererBase());
	}

	InitializeCommandBuffers();
	UpdateCommandBuffers = true;
}

uint32_t Renderer::Draw(GLFWwindow* window, FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
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

	VkCommandBufferBeginInfo CommandBufferInfo = {};
	CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(RenderCommandBuffer[DrawFrame], &CommandBufferInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	DrawToTextureRenderPass(skybox, MeshList);
	MainRenderPass(framebuffer, skybox, MeshList);

	if (vkEndCommandBuffer(RenderCommandBuffer[DrawFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	VkSemaphore waitSemaphores[] = { vulkanSemaphores[currentFrame].ImageAcquiredSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { vulkanSemaphores[currentFrame].RenderCompleteSemaphore };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &RenderCommandBuffer[DrawFrame];
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

void Renderer::DrawToTextureRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = textureRenderer.RenderPass;
	renderPassInfo.framebuffer = textureRenderer.SwapChainFramebuffers[DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(RenderCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	textureRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList[0]);
	textureRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList[1]);
	textureRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList[2]);
	textureRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, skybox);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void Renderer::MainRenderPass(FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	//clearValues[1].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = forwardRenderer.RenderPass;
	renderPassInfo.framebuffer = forwardRenderer.SwapChainFramebuffers[DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(RenderCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


	//VkViewport viewports[2];
	//// Left
	//viewports[0] = { 0, 0, (float)SwapChain.GetSwapChainResolution().width / 2.0f, (float)SwapChain.GetSwapChainResolution().height, 0.0, 1.0f };
	//// Right
	//viewports[1] = { (float)SwapChain.GetSwapChainResolution().width / 2.0f, 0, (float)SwapChain.GetSwapChainResolution().width / 2.0f, (float)SwapChain.GetSwapChainResolution().height, 0.0, 1.0f };

	//vkCmdSetViewport(RenderCommandBuffer[DrawFrame], 0, 2, viewports);

	/*VkRect2D scissorRects[2]; 
	scissorRects[0].extent.width = SwapChain.GetSwapChainResolution().width / 2;
	scissorRects[0].extent.height = SwapChain.GetSwapChainResolution().height;
	scissorRects[0].extent.width = 0;
	scissorRects[0].extent.height = 0;

	scissorRects[1].extent.width = SwapChain.GetSwapChainResolution().width / 2;
	scissorRects[1].extent.height = SwapChain.GetSwapChainResolution().height;
	scissorRects[1].extent.width = SwapChain.GetSwapChainResolution().width / 2;
	scissorRects[1].extent.height = 0;


	vkCmdSetScissor(RenderCommandBuffer[DrawFrame], 0, 2, scissorRects);

	vkCmdSetLineWidth(RenderCommandBuffer[DrawFrame], 1.0f);

	vkCmdSetViewport(RenderCommandBuffer[DrawFrame], 0, 1, viewports);*/

	forwardRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList);
	forwardRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, skybox);
	forwardRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, framebuffer);
	//vkCmdNextSubpass(RenderCommandBuffer[DrawFrame], VK_SUBPASS_CONTENTS_INLINE);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), RenderCommandBuffer[DrawFrame]);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void Renderer::DestoryVulkan()
{
	auto vulkanRenderer = static_cast<VulkanRenderer>(*this);

	guiDebugger.ShutDown(Device);

	forwardRenderer.Destroy(vulkanRenderer);
	//textureRenderer.Destroy(vulkanRenderer);

	//DebugLightPipeline.Destroy();
	//DebugCollisionPipeline.Destroy();
	//MeshviewPipeline.Destroy();
	//SkyboxPipeline.Destroy();

	SwapChain.Destroy(Device);

	vkDestroyCommandPool(Device, RenderCommandPool, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vulkanSemaphores[i].Destory(Device);
		vkDestroyFence(Device, inFlightFences[i], nullptr);

		inFlightFences[i] = VK_NULL_HANDLE;
	}

	vkDestroyDevice(Device, nullptr);

	RenderCommandPool = VK_NULL_HANDLE;

	VulkanRenderer::Destory();
}
