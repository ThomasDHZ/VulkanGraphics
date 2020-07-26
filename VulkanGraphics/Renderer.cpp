#include "Renderer.h"
#include <set>

Renderer::Renderer() : VulkanRenderer()
{
}

Renderer::Renderer(GLFWwindow* window) : VulkanRenderer(window)
{
	forwardRenderer = ForwardRenderer(*GetVulkanRendererBase());
	textureRenderer = TextureRenderer(*GetVulkanRendererBase());
	frameBufferRenderer = FramebufferRenderer(*GetVulkanRendererBase());
	InitializeGUIDebugger(window);
}

Renderer::~Renderer()
{
}

void Renderer::CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = RenderCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)RenderCommandBuffer.size();

	if (vkAllocateCommandBuffers(Device, &allocInfo, RenderCommandBuffer.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < RenderCommandBuffer.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(RenderCommandBuffer[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = forwardRenderer.RenderPass;
		renderPassInfo.framebuffer = forwardRenderer.SwapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(RenderCommandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		VkBuffer vertexBuffers[] = { MeshList[0].VertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline(RenderCommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRenderer.forwardRendereringPipeline.ShaderPipeline);
		vkCmdBindVertexBuffers(RenderCommandBuffer[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(RenderCommandBuffer[i], MeshList[0].IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(RenderCommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRenderer.forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &MeshList[0].DescriptorSets[i], 0, nullptr);
		vkCmdDrawIndexed(RenderCommandBuffer[i], static_cast<uint32_t>(MeshList[0].IndexSize), 1, 0, 0, 0);
		vkCmdEndRenderPass(RenderCommandBuffer[i]);

		if (vkEndCommandBuffer(RenderCommandBuffer[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
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

void Renderer::UpdateSwapChain(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
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

	forwardRenderer.UpdateSwapChain(*GetVulkanRendererBase());
	textureRenderer.UpdateSwapChain(*GetVulkanRendererBase());
	frameBufferRenderer.UpdateSwapChain(*GetVulkanRendererBase());

	InitializeCommandBuffers();
	CMDBuffer(frameBuffer, skybox, MeshList);
}

uint32_t Renderer::Draw(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	vkWaitForFences(Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(Device, SwapChain.GetSwapChain(), UINT64_MAX, vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &DrawFrame);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		UpdateSwapChain(window, frameBuffer, skybox, MeshList);
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
	MainRenderPass(skybox, MeshList);
	FrameBufferRenderPass(frameBuffer, skybox, MeshList);

	if (vkEndCommandBuffer(RenderCommandBuffer[DrawFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	//if (vkBeginCommandBuffer(RenderCommandBuffer[DrawFrame], &CommandBufferInfo) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to begin recording command buffer!");
	//}

	//std::array<VkClearValue, 2> clearValues{};
	//clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	//clearValues[1].depthStencil = { 1.0f, 0 };

	//VkRenderPassBeginInfo renderPassInfo{};
	//renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//renderPassInfo.renderPass = forwardRenderer2.RenderPass;
	//renderPassInfo.framebuffer = forwardRenderer2.SwapChainFramebuffers[DrawFrame];
	//renderPassInfo.renderArea.offset = { 0, 0 };
	//renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	//renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	//renderPassInfo.pClearValues = clearValues.data();

	//vkCmdBeginRenderPass(RenderCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), RenderCommandBuffer[DrawFrame]);
	//vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);

	//if (vkEndCommandBuffer(RenderCommandBuffer[DrawFrame]) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to record command buffer!");
	//}


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
		UpdateSwapChain(window, frameBuffer, skybox, MeshList);
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
	textureRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, MeshList[0]);
	//textureRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, MeshList[1]);
	//textureRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, MeshList[2]);
	textureRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, skybox);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void Renderer::MainRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
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

	forwardRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, MeshList);
	forwardRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, skybox);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void Renderer::FrameBufferRenderPass(FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
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
	forwardRenderer.Draw(*GetVulkanRendererBase(), DrawFrame, framebuffer);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), RenderCommandBuffer[DrawFrame]);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void Renderer::DestoryVulkan()
{
	auto vulkanRenderer = static_cast<VulkanRenderer>(*this);

	guiDebugger.ShutDown(Device);

	forwardRenderer.Destroy(vulkanRenderer);
	textureRenderer.Destroy(vulkanRenderer);
	frameBufferRenderer.Destroy(vulkanRenderer);

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
