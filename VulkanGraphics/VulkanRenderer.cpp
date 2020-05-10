#include "VulkanRenderer.h"
#include <set>

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::VulkanRenderer(GLFWwindow* window)
{
	InitializeVulkan(window);
	swapChain = VulkanSwapChain(window, Device, PhysicalDevice, Surface);
	InitializeRenderPass();
	InitializeFramebuffers();
	InitializeCommandBuffers();
	InitializeSyncObjects();

	GraphicsPipeline = ForwardRenderingPipeline(swapChain.GetSwapChainResolution(), RenderPass, Device);
	MeshviewPipeline = WireFramePipeline(swapChain.GetSwapChainResolution(), RenderPass, Device);
	SkyboxPipeline = SkyBoxPipeline(swapChain.GetSwapChainResolution(), RenderPass, Device);

	VulkanDevice vd = {};
	vd.Device = Device;
	vd.CommandPool = MainCommandPool;
	vd.GraphicsQueue = GraphicsQueue;
	vd.PhysicalDevice = PhysicalDevice;
	vd.SwapChainSize = 3;
	framebuffer = FrameBuffer(vd, swapChain.GetSwapChainResolution(), RenderPass, ColorAttachment, DepthAttachment);
}

VulkanRenderer::~VulkanRenderer()
{
}

VkFormat VulkanRenderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(PhysicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat VulkanRenderer::findDepthFormat() 
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

std::vector<const char*> VulkanRenderer::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice GPUDevice)
{
	FindQueueFamilies(GPUDevice, Surface);

	bool extensionsSupported = checkDeviceExtensionSupport(GPUDevice);


	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(GPUDevice, &supportedFeatures);

	uint32_t GPUSurfaceFormatCount;
	uint32_t GPUPresentModeCount;
	std::vector<VkSurfaceFormatKHR> GPUSwapChainFormatCapabilities;
	std::vector<VkPresentModeKHR> GPUPresentModesList;

	vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, nullptr);
	if (GPUSurfaceFormatCount != 0)
	{
		GPUSwapChainFormatCapabilities.resize(GPUSurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GPUDevice, Surface, &GPUSurfaceFormatCount, GPUSwapChainFormatCapabilities.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, nullptr);
	if (GPUPresentModeCount != 0)
	{
		GPUPresentModesList.resize(GPUPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GPUDevice, Surface, &GPUPresentModeCount, GPUPresentModesList.data());
	}


	return GraphicsFamily != -1 &&
		PresentFamily != -1 &&
		extensionsSupported &&
		GPUSwapChainFormatCapabilities.size() != 0 &&
		GPUPresentModesList.size() != 0 &&
		supportedFeatures.samplerAnisotropy;
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(GPUDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void VulkanRenderer::FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			GraphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, Surface, &presentSupport);

		if (presentSupport) {
			PresentFamily = i;
		}

		if (GraphicsFamily != -1 &&
			PresentFamily != -1)
		{
			break;
		}

		i++;
	}
}

void VulkanRenderer::InitializeVulkan(GLFWwindow* window)
{
	bool DebuggerAvalible = false;
	uint32_t LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	VulkanLayers.resize(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, VulkanLayers.data());

	for (const char* LayerName : validationLayers)
	{
		if (LayerName == "VK_LAYER_KHRONOS_validation")
		{
			DebuggerAvalible = true;
			break;
		}
	}

	auto ExtensionList = getRequiredExtensions();

	VkApplicationInfo VulkanInfo = {};
	VulkanInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanInfo.pApplicationName = "Vulkan Graphics";
	VulkanInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanInfo.pEngineName = "No Engine";
	VulkanInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo VulkanCreateInfo = {};
	VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	VulkanCreateInfo.pApplicationInfo = &VulkanInfo;
	VulkanCreateInfo.enabledExtensionCount = static_cast<uint32_t>(ExtensionList.size());
	VulkanCreateInfo.ppEnabledExtensionNames = ExtensionList.data();

	if (DebuggerAvalible)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

		VulkanCreateInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
		VulkanCreateInfo.ppEnabledLayerNames = validationLayers.data();
		VulkanCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}
	else
	{
		VulkanCreateInfo.enabledLayerCount = 0;
		VulkanCreateInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&VulkanCreateInfo, nullptr, &Instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	VulkanDebug.SetUpDebugger(Instance);

	if (glfwCreateWindowSurface(Instance, window, nullptr, &Surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

	for (const auto& gpudevice : devices) {
		if (isDeviceSuitable(gpudevice))
		{
			PhysicalDevice = gpudevice;
			break;
		}
	}

	if (PhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { static_cast<uint32_t>(GraphicsFamily), static_cast<uint32_t>(PresentFamily) };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (DebuggerAvalible)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(Device, GraphicsFamily, 0, &GraphicsQueue);
	vkGetDeviceQueue(Device, PresentFamily, 0, &PresentQueue);
}

void VulkanRenderer::InitializeRenderPass()
{
	std::array<VkAttachmentDescription, 3> attachments{};

	// Swap chain image color attachment
	// Will be transitioned to present layout
	attachments[0].format = swapChain.GetSwapChainImageFormat().format;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Input attachments
	// These will be written in the first subpass, transitioned to input attachments 
	// and then read in the secod subpass

	// Color
	attachments[1].format = VK_FORMAT_R8G8B8A8_UNORM;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	// Depth
	attachments[2].format = findDepthFormat();
	attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[2].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	std::array<VkSubpassDescription, 2> subpassDescriptions{};

	/*
		First subpass
		Fill the color and depth attachments
	*/
	VkAttachmentReference colorReference = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthReference = { 2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].colorAttachmentCount = 1;
	subpassDescriptions[0].pColorAttachments = &colorReference;
	subpassDescriptions[0].pDepthStencilAttachment = &depthReference;

	/*
		Second subpass
		Input attachment read and swap chain color attachment write
	*/

	// Color reference (target) for this sub pass is the swap chain color attachment
	VkAttachmentReference colorReferenceSwapchain = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	subpassDescriptions[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[1].colorAttachmentCount = 1;
	subpassDescriptions[1].pColorAttachments = &colorReferenceSwapchain;

	// Color and depth attachment written to in first sub pass will be used as input attachments to be read in the fragment shader
	VkAttachmentReference inputReferences[2];
	inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
	inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

	// Use the attachments filled in the first pass as input attachments
	subpassDescriptions[1].inputAttachmentCount = 2;
	subpassDescriptions[1].pInputAttachments = inputReferences;

	/*
		Subpass dependencies for layout transitions
	*/
	std::array<VkSubpassDependency, 3> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// This dependency transitions the input attachment from color attachment to shader read
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = 1;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[2].srcSubpass = 0;
	dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo renderPassInfoCI{};
	renderPassInfoCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfoCI.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfoCI.pAttachments = attachments.data();
	renderPassInfoCI.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
	renderPassInfoCI.pSubpasses = subpassDescriptions.data();
	renderPassInfoCI.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfoCI.pDependencies = dependencies.data();

	if (vkCreateRenderPass(Device, &renderPassInfoCI, nullptr, &RenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void VulkanRenderer::InitializeFramebuffers()
{
	ColorAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkColorAttachment, swapChain.GetSwapChainResolution().width, swapChain.GetSwapChainResolution().height);
	DepthAttachment = InputAttachment(Device, PhysicalDevice, AttachmentType::VkDepthAttachemnt, swapChain.GetSwapChainResolution().width, swapChain.GetSwapChainResolution().height);

	SwapChainFramebuffers.resize(swapChain.GetSwapChainImageCount());

	for (size_t i = 0; i < swapChain.GetSwapChainImageCount(); i++) {
		std::array<VkImageView, 3> attachments =
		{
			swapChain.GetSwapChainImageViews()[i],
			ColorAttachment.AttachmentImageView,
			DepthAttachment.AttachmentImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChain.GetSwapChainResolution().width;
		framebufferInfo.height = swapChain.GetSwapChainResolution().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void VulkanRenderer::InitializeCommandBuffers()
{
	MainCommandBuffer.resize(swapChain.GetSwapChainImageCount());
	SecondaryCommandBuffers.resize(swapChain.GetSwapChainImageCount());

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

void VulkanRenderer::InitializeSyncObjects()
{
	vulkanSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChain.GetSwapChainImageCount(), VK_NULL_HANDLE);

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

void VulkanRenderer::UpdateSwapChain(GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(Device);

	DepthAttachment.UpdateFrameBuffer(Device);

	for (auto framebuffer : SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(Device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(Device, SecondaryCommandPool, static_cast<uint32_t>(SecondaryCommandBuffers.size()), SecondaryCommandBuffers.data());
	vkDestroyPipeline(Device, GraphicsPipeline.ShaderPipeline, nullptr);
	vkDestroyPipelineLayout(Device, GraphicsPipeline.ShaderPipelineLayout, nullptr);
	vkDestroyPipeline(Device, MeshviewPipeline.ShaderPipeline, nullptr);
	vkDestroyPipelineLayout(Device, MeshviewPipeline.ShaderPipelineLayout, nullptr);
	vkDestroyPipeline(Device, SkyboxPipeline.ShaderPipeline, nullptr);
	vkDestroyPipelineLayout(Device, SkyboxPipeline.ShaderPipelineLayout, nullptr);
	framebuffer.DestroySwapChainStage();

	for (auto imageView : swapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(Device, imageView, nullptr);
	}

	vkDestroyCommandPool(Device, MainCommandPool, nullptr);
	vkDestroyCommandPool(Device, SecondaryCommandPool, nullptr);
	vkDestroySwapchainKHR(Device, swapChain.GetSwapChain(), nullptr);

	swapChain.UpdateSwapChain(window, Device, PhysicalDevice, Surface);
	GraphicsPipeline.UpdateGraphicsPipeLine(swapChain.GetSwapChainResolution(), RenderPass, Device);
	MeshviewPipeline.UpdateGraphicsPipeLine(swapChain.GetSwapChainResolution(), RenderPass, Device);
	SkyboxPipeline.UpdateGraphicsPipeLine(swapChain.GetSwapChainResolution(), RenderPass, Device);
	InitializeFramebuffers();
	InitializeCommandBuffers();

	framebuffer.RecreateSwapChainStage(swapChain.GetSwapChainResolution(), RenderPass, ColorAttachment, DepthAttachment);
	UpdateCommandBuffers = true;
}

uint32_t VulkanRenderer::StartFrame(GLFWwindow* window)
{
	vkWaitForFences(Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(Device, swapChain.GetSwapChain(), UINT64_MAX, vulkanSemaphores[currentFrame].ImageAcquiredSemaphore, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		UpdateSwapChain(window);
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	return imageIndex;
}
void VulkanRenderer::EndFrame(GLFWwindow* window, uint32_t imageIndex)
{
	std::array<VkClearValue, 3> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[2].depthStencil = { 1.0f, 0 };

	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(Device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = RenderPass;
	renderPassInfo.framebuffer = SwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	VkCommandBufferBeginInfo CommandBufferInfo = {};
	CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	vkBeginCommandBuffer(MainCommandBuffer[imageIndex], &CommandBufferInfo);
	vkCmdBeginRenderPass(MainCommandBuffer[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	vkCmdExecuteCommands(MainCommandBuffer[imageIndex], RunCommandBuffers.size(), RunCommandBuffers.data());
	vkCmdNextSubpass(MainCommandBuffer[imageIndex], VK_SUBPASS_CONTENTS_INLINE);
	framebuffer.Draw(MainCommandBuffer[imageIndex], imageIndex);
	vkCmdEndRenderPass(MainCommandBuffer[imageIndex]);
	vkEndCommandBuffer(MainCommandBuffer[imageIndex]);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { vulkanSemaphores[currentFrame].ImageAcquiredSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &MainCommandBuffer[imageIndex];

	VkSemaphore signalSemaphores[] = { vulkanSemaphores[currentFrame].RenderCompleteSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(Device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(GraphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

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

void VulkanRenderer::DestoryVulkan()
{
	DepthAttachment.UpdateFrameBuffer(Device);

	GraphicsPipeline.Destroy();
	MeshviewPipeline.Destroy();
	SkyboxPipeline.Destroy();

	swapChain.Destroy(Device);

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
	Device = VK_NULL_HANDLE;

	VulkanDebug.CleanUp(Instance);

	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroyInstance(Instance, nullptr);
}
