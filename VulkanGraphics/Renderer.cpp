#include "Renderer.h"
#include <set>

Renderer::Renderer() : VulkanRenderer()
{
}

Renderer::Renderer(GLFWwindow* window) : VulkanRenderer(window)
{
	forwardRenderer = ForwardRenderer(Device, PhysicalDevice, SwapChain.GetSwapChainResolution(), SwapChain.GetSwapChainImageViews());
	textureRenderer = TextureRenderer(Device, PhysicalDevice, SwapChain.GetSwapChainResolution(), SwapChain.GetSwapChainImageViews());
	InitializeGUIDebugger(window);

	DebugLightPipeline = DebugLightRenderingPipeline(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	DebugCollisionPipeline = CollisionDebugPipeline(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	MeshviewPipeline = WireFramePipeline(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	SkyboxPipeline = SkyBoxPipeline(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
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

	DebugLightPipeline.UpdateSwapChain();
	DebugCollisionPipeline.UpdateSwapChain();
	MeshviewPipeline.UpdateSwapChain();
	SkyboxPipeline.UpdateSwapChain();

	DebugLightPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	DebugCollisionPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	MeshviewPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	SkyboxPipeline.UpdateGraphicsPipeLine(SwapChain.GetSwapChainResolution(), forwardRenderer.RenderPass, Device);
	{

		forwardRenderer.DepthTexture.Delete(Device);

		for (auto framebuffer : forwardRenderer.SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(Device, framebuffer, nullptr);
		}

		vkDestroyPipeline(Device, forwardRenderer.RendererPipeline, nullptr);
		vkDestroyPipelineLayout(Device, forwardRenderer.RendererLayout, nullptr);

		forwardRenderer.RendererPipeline = VK_NULL_HANDLE;
		forwardRenderer.RendererLayout = VK_NULL_HANDLE;

		forwardRenderer.UpdateSwapChain(Device, PhysicalDevice, SwapChain.GetSwapChainResolution(), SwapChain.GetSwapChainImageViews());
	}
	{

		textureRenderer.DepthTexture.Delete(Device);
		textureRenderer.ColorTexture.Delete(Device);
		for (auto framebuffer : textureRenderer.SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(Device, framebuffer, nullptr);
		}

		vkDestroyPipeline(Device, textureRenderer.RendererPipeline, nullptr);
		vkDestroyPipelineLayout(Device, textureRenderer.RendererLayout, nullptr);

		textureRenderer.RendererPipeline = VK_NULL_HANDLE;
		textureRenderer.RendererLayout = VK_NULL_HANDLE;


		textureRenderer.DepthTexture = RendererDepthTexture(Device, PhysicalDevice, SwapChain.GetSwapChainResolution());
		textureRenderer.ColorTexture = RendererColorTexture(Device, PhysicalDevice, SwapChain.GetSwapChainResolution());
		textureRenderer.CreateRenderingPipeline(Device, SwapChain.GetSwapChainResolution());
		textureRenderer.CreateRendererFramebuffers(Device, SwapChain.GetSwapChainResolution(), SwapChain.GetSwapChainImageViews());
	}

	InitializeCommandBuffers();
	UpdateCommandBuffers = true;
}

uint32_t Renderer::Draw(GLFWwindow* window, std::vector<Mesh2>& MeshList)
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

	textureRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList[0]);
	forwardRenderer.Draw(SwapChain.GetSwapChainResolution(), RenderCommandBuffer[DrawFrame], DrawFrame, MeshList);

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

void Renderer::DestoryVulkan()
{
	guiDebugger.ShutDown(Device);

	forwardRenderer.Destroy(Device);
	textureRenderer.Destroy(Device);

	DebugLightPipeline.Destroy();
	DebugCollisionPipeline.Destroy();
	MeshviewPipeline.Destroy();
	SkyboxPipeline.Destroy();

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
