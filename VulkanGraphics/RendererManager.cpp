#include "RendererManager.h"
#include <set>

RendererManager::RendererManager() : VulkanRenderer()
{
}

RendererManager::RendererManager(GLFWwindow* window) : VulkanRenderer(window)
{
	forwardRenderer = ForwardRenderer(*GetVulkanRendererBase());
	InitializeGUIDebugger(window);


	textureRenderer = TextureRenderer(*GetVulkanRendererBase());
	//frameBufferRenderer = FramebufferRenderer(*GetVulkanRendererBase());
	//shadowRenderer = ShadowRenderer(*GetVulkanRendererBase());
}

RendererManager::~RendererManager()
{
}

//void RendererManager::CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh>& MeshList)
//{
//	VkCommandBufferAllocateInfo allocInfo{};
//	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//	allocInfo.commandPool = RenderCommandPool;
//	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//	allocInfo.commandBufferCount = (uint32_t)RenderCommandBuffer.size();
//
//	if (vkAllocateCommandBuffers(Device, &allocInfo, RenderCommandBuffer.data()) != VK_SUCCESS) {
//		throw std::runtime_error("failed to allocate command buffers!");
//	}
//
//	for (size_t i = 0; i < RenderCommandBuffer.size(); i++) {
//		VkCommandBufferBeginInfo beginInfo{};
//		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//
//		if (vkBeginCommandBuffer(RenderCommandBuffer[i], &beginInfo) != VK_SUCCESS) {
//			throw std::runtime_error("failed to begin recording command buffer!");
//		}
//
//		VkRenderPassBeginInfo renderPassInfo{};
//		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//		renderPassInfo.renderPass = forwardRenderer.RenderPass;
//		renderPassInfo.framebuffer = forwardRenderer.SwapChainFramebuffers[i];
//		renderPassInfo.renderArea.offset = { 0, 0 };
//		renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
//
//		std::array<VkClearValue, 2> clearValues{};
//		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//		clearValues[1].depthStencil = { 1.0f, 0 };
//
//		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//		renderPassInfo.pClearValues = clearValues.data();
//
//		vkCmdBeginRenderPass(RenderCommandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//		VkBuffer vertexBuffers[] = { MeshList[0].VertexBuffer };
//		VkDeviceSize offsets[] = { 0 };
//
//	/*	vkCmdBindPipeline(RenderCommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRenderer.forwardRendereringPipeline.ShaderPipeline);
//		vkCmdBindVertexBuffers(RenderCommandBuffer[i], 0, 1, vertexBuffers, offsets);
//		vkCmdBindIndexBuffer(RenderCommandBuffer[i], MeshList[0].IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
//		vkCmdBindDescriptorSets(RenderCommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRenderer.forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &MeshList[0].DescriptorSets[i], 0, nullptr);
//		vkCmdDrawIndexed(RenderCommandBuffer[i], static_cast<uint32_t>(MeshList[0].IndexSize), 1, 0, 0, 0);*/
//		vkCmdEndRenderPass(RenderCommandBuffer[i]);
//
//		if (vkEndCommandBuffer(RenderCommandBuffer[i]) != VK_SUCCESS) {
//			throw std::runtime_error("failed to record command buffer!");
//		}
//	}
//}


void RendererManager::InitializeGUIDebugger(GLFWwindow* window)
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

void RendererManager::UpdateSwapChain(GLFWwindow* window)
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
	//frameBufferRenderer.UpdateSwapChain(*GetVulkanRendererBase());
	//shadowRenderer.UpdateSwapChain(*GetVulkanRendererBase());

	InitializeCommandBuffers();
}

uint32_t RendererManager::Draw(GLFWwindow* window, std::vector<Mesh>& MeshList, SkyBoxMesh skybox, DebugLightMesh debugLight)
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

	//ShadowRenderPass(MeshList);
	DrawToTextureRenderPass(MeshList);
	MainRenderPass(MeshList, skybox, debugLight);
	//FrameBufferRenderPass(frameBuffer, skybox, MeshList);

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
		UpdateSwapChain(window);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RendererManager::DrawToTextureRenderPass(std::vector<Mesh>& MeshList)
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
	for (auto mesh : MeshList)
	{
		if (mesh.RenderBitFlags & RendererBitFlag::RenderOnTexturePass)
		{
			textureRenderer.Draw(*GetVulkanRendererBase(), textureRenderer.forwardRendereringPipeline, mesh);
		}
	}
	//textureRenderer.Draw(*GetVulkanRendererBase(), textureRenderer.skyboxPipeline, skybox);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void RendererManager::MainRenderPass(std::vector<Mesh>& MeshList, SkyBoxMesh skybox, DebugLightMesh debugLight)
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
	for (auto mesh : MeshList)
	{
		if (mesh.RenderBitFlags & RendererBitFlag::RenderOnMainPass)
		{
			if (Settings.ShowMeshLines)
			{
				forwardRenderer.Draw(*GetVulkanRendererBase(), forwardRenderer.wireFramePipeline, mesh);
			}
			else
			{
				forwardRenderer.Draw(*GetVulkanRendererBase(), forwardRenderer.forwardRendereringPipeline, mesh);
			}
		}
	}
	forwardRenderer.Draw(*GetVulkanRendererBase(), forwardRenderer.skyboxPipeline, skybox);
	forwardRenderer.Draw(*GetVulkanRendererBase(), forwardRenderer.DebugLightPipeline, debugLight);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), RenderCommandBuffer[DrawFrame]);
	vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void RendererManager::FrameBufferRenderPass(std::vector<Mesh>& MeshList)
{
	//std::array<VkClearValue, 2> clearValues{};
	//clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	//clearValues[1].depthStencil = { 1.0f, 0 };

	//VkRenderPassBeginInfo renderPassInfo{};
	//renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//renderPassInfo.renderPass = forwardRenderer.RenderPass;
	//renderPassInfo.framebuffer = forwardRenderer.SwapChainFramebuffers[DrawFrame];
	//renderPassInfo.renderArea.offset = { 0, 0 };
	//renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	//renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	//renderPassInfo.pClearValues = clearValues.data();

	//vkCmdBeginRenderPass(RenderCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	////forwardRenderer.Draw(*GetVulkanRendererBase(), frameBufferRenderer.frameBufferPipeline, framebuffer);
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), RenderCommandBuffer[DrawFrame]);
	//vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void RendererManager::ShadowRenderPass(std::vector<Mesh>& MeshList)
{
	//std::array<VkClearValue, 1> clearValues{};
	//clearValues[0].depthStencil = { 1.0f, 0 };

	//VkRenderPassBeginInfo renderPassInfo{};
	//renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//renderPassInfo.renderPass = shadowRenderer.RenderPass;
	//renderPassInfo.framebuffer = shadowRenderer.SwapChainFramebuffers[DrawFrame];
	//renderPassInfo.renderArea.offset = { 0, 0 };
	//renderPassInfo.renderArea.extent = SwapChain.GetSwapChainResolution();
	//renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	//renderPassInfo.pClearValues = clearValues.data();

	//vkCmdBeginRenderPass(RenderCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	//for (auto mesh : MeshList)
	//{
	//	if (mesh.RenderBitFlags & RendererBitFlag::RenderShadow)
	//	{
	//		shadowRenderer.Draw(*GetVulkanRendererBase(), shadowRenderer.forwardRendereringPipeline, mesh);
	//	}
	//}
	//vkCmdEndRenderPass(RenderCommandBuffer[DrawFrame]);
}

void RendererManager::DestoryVulkan()
{
	guiDebugger.ShutDown(Device);

	forwardRenderer.Destroy(*GetVulkanRendererBase());
	textureRenderer.Destroy(*GetVulkanRendererBase());
	//frameBufferRenderer.Destroy(*GetVulkanRendererBase());
	//shadowRenderer.Destroy(*GetVulkanRendererBase());

	VulkanRenderer::Destory();
}
