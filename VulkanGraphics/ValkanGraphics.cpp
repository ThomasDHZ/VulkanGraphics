#include "ValkanGraphics.h"
#include <set>
#include <algorithm> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ValkanGraphics::ValkanGraphics(unsigned int width, unsigned int height, const char* windowName) : EnableValidationLayers(true)
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
	DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	Window = VulkanWindow(width, height, windowName);

	SetUpVulkanInstance();
	SetUpDebugger();
	SetUpSurface();
	SetUpPhysicalDevice();
	SetUpLogicalDevice();
	SetUpSwapChain();
	SetUpImageViews();
	SetUpRenderPass();
	SetDescriptorSetLayout();
	SetUpGraphicsPipeLine();
	SetUpFrameBuffers();
	SetUpCommandPool();
	SetUpTextureImage();
	SetUpTextureImageView();
	SetUpTextureSampler();
	SetUpVertexBuffers();
	SetUpIndexBuffers();
	SetUpUniformBuffer();
	SetUpDescriptorPool();
	SetUpDescriptorSets();
	SetUpCommandBuffers();
	SetUpSyncObjects();
	
}

ValkanGraphics::~ValkanGraphics()
{
	CleanUpSwapChain();

	vkDestroySampler(GPUInfo.Device, TextureSampler, nullptr);
	vkDestroyImageView(GPUInfo.Device, TextureImageView, nullptr);
	vkDestroyImage(GPUInfo.Device, TextureImage, nullptr);
	vkFreeMemory(GPUInfo.Device, TextureImageMemory, nullptr);

	for (size_t i = 0; i < SwapChainImages.size(); i++) 
	{
		vkDestroyBuffer(GPUInfo.Device, uniformBuffers[i], nullptr);
		vkFreeMemory(GPUInfo.Device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(GPUInfo.Device, DescriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(GPUInfo.Device, DescriptorSetLayout, nullptr);
	vkDestroyBuffer(GPUInfo.Device, IndexBuffer, nullptr);
	vkFreeMemory(GPUInfo.Device, IndexBufferMemory, nullptr);
	vkDestroyBuffer(GPUInfo.Device, VertexBuffer, nullptr);
	vkFreeMemory(GPUInfo.Device, VertexBufferMemory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(GPUInfo.Device, RenderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(GPUInfo.Device, ImageAvailableSemaphores[i], nullptr);
		vkDestroyFence(GPUInfo.Device, InFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(GPUInfo.Device, CommandPool, nullptr);
	vkDestroyDevice(GPUInfo.Device, nullptr);
	VulkanDebug.CleanUp(VulkanInstance);
	vkDestroySurfaceKHR(VulkanInstance, VulkanSurface, nullptr);
	vkDestroyInstance(VulkanInstance, nullptr);
	Window.CleanUp();
}

void ValkanGraphics::SetUpVulkanInstance()
{
	if (EnableValidationLayers && !ValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available.");
	}

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtenstion;

	glfwExtenstion = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkApplicationInfo VKInfo = {};
	VKInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VKInfo.pApplicationName = "Valkan Graphics Testing";
	VKInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VKInfo.pEngineName = "No Engine";
	VKInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VKInfo.apiVersion = VK_API_VERSION_1_1;

	auto extensions = GetRequiredExtensions();

	VkInstanceCreateInfo CreateVKInfo = {};
	CreateVKInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateVKInfo.pApplicationInfo = &VKInfo;
	CreateVKInfo.enabledExtensionCount = glfwExtensionCount;
	CreateVKInfo.ppEnabledExtensionNames = glfwExtenstion;
	CreateVKInfo.enabledExtensionCount = static_cast<unsigned int>(extensions.size());
	CreateVKInfo.ppEnabledExtensionNames = extensions.data();


	if (EnableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
		VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

		CreateVKInfo.enabledLayerCount = static_cast<unsigned int>(ValidationLayers.size());
		CreateVKInfo.ppEnabledLayerNames = ValidationLayers.data();
		CreateVKInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
	}
	else
	{
		CreateVKInfo.enabledLayerCount = 0;
	}

	VkResult Result = vkCreateInstance(&CreateVKInfo, nullptr, &VulkanInstance);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan instance. Error:" + Result);
	}
}

void ValkanGraphics::SetUpDebugger()
{
	if (!EnableValidationLayers)
	{
		return;
	}

	VulkanDebug.SetUpDebugger(VulkanInstance);
}

void ValkanGraphics::SetUpPhysicalDevice()
{
	unsigned int GPUCount;
	vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, nullptr);

	if (GPUCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> GPUDevices(GPUCount);
	vkEnumeratePhysicalDevices(VulkanInstance, &GPUCount, GPUDevices.data());

	for (const auto& device : GPUDevices)
	{
		if (IsDeviceCompatable(device))
		{
			GPUInfo.PhysicalDevice = device;
		}

		if (GPUInfo.PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}
}

void ValkanGraphics::SetUpLogicalDevice()
{
	VulkanQueueFamily indices = FindQueueFamilies(GPUInfo.PhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
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

	createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

	if (EnableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		createInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(GPUInfo.PhysicalDevice, &createInfo, nullptr, &GPUInfo.Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(GPUInfo.Device, indices.GraphicsFamily.value(), 0, &GraphicsQueue);
	vkGetDeviceQueue(GPUInfo.Device, indices.PresentationFamily.value(), 0, &PresentQueue);
}

void ValkanGraphics::SetUpSurface()
{
	VkResult Result = glfwCreateWindowSurface(VulkanInstance, Window.GetWindowPtr(), nullptr, &VulkanSurface);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface.");
	}
}

void ValkanGraphics::SetUpSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(GPUInfo.PhysicalDevice);
	VkSurfaceFormatKHR surfaceFormat = SetSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = SetSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = SetSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = VulkanSurface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VulkanQueueFamily indices = FindQueueFamilies(GPUInfo.PhysicalDevice);
	uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentationFamily.value() };

	if (indices.GraphicsFamily != indices.PresentationFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	try
	{
		VkResult Result = vkCreateSwapchainKHR(GPUInfo.Device, &createInfo, nullptr, &SwapChain);
		if (Result != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	vkGetSwapchainImagesKHR(GPUInfo.Device, SwapChain, &imageCount, nullptr);
	SwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(GPUInfo.Device, SwapChain, &imageCount, SwapChainImages.data());

	SwapChainImageFormat = surfaceFormat.format;
	SwapChainExtent = extent;
}

void ValkanGraphics::SetUpImageViews()
{
	SwapChainImageViews.resize(SwapChainImages.size());
	for (size_t i = 0; i < SwapChainImages.size(); i++) {
		SwapChainImageViews[i] = CreateImageView(SwapChainImages[i], SwapChainImageFormat);
	}
}

void ValkanGraphics::SetUpRenderPass()
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = SwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(GPUInfo.Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void ValkanGraphics::SetDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding UBOLayoutBinding = {};
	UBOLayoutBinding.binding = 0;
	UBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	UBOLayoutBinding.descriptorCount = 1;
	UBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	UBOLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding SamplerLayoutBinding = {};
	SamplerLayoutBinding.binding = 1;
	SamplerLayoutBinding.descriptorCount = 1;
	SamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	SamplerLayoutBinding.pImmutableSamplers = nullptr;
	SamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { UBOLayoutBinding, SamplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo LayoutInfo = {};
	LayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	LayoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	LayoutInfo.pBindings = bindings.data();

	VkResult Result = vkCreateDescriptorSetLayout(GPUInfo.Device, &LayoutInfo, nullptr, &DescriptorSetLayout);
	if (Result != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to create descriptor set layout.");
	}
}

void ValkanGraphics::SetUpGraphicsPipeLine()
{
	VkShaderModule VertexShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/vert.spv");
	VkShaderModule FragmentShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/frag.spv");

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineShaderStageCreateInfo VertexShaderStageInfo = {};
	VertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	VertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	VertexShaderStageInfo.module = VertexShaderModule;
	VertexShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FragmentShaderStageInfo = {};
	FragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FragmentShaderStageInfo.module = FragmentShaderModule;
	FragmentShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexShaderStageInfo, FragmentShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.vertexBindingDescriptionCount = 1;
	VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	VertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	VertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};
	InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport = {};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = (float)SwapChainExtent.width;
	Viewport.height = (float)SwapChainExtent.height;
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D Scissor = {};
	Scissor.offset = { 0, 0 };
	Scissor.extent = SwapChainExtent;

	VkPipelineViewportStateCreateInfo ViewportState = {};
	ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportState.viewportCount = 1;
	ViewportState.pViewports = &Viewport;
	ViewportState.scissorCount = 1;
	ViewportState.pScissors = &Scissor;

	VkPipelineRasterizationStateCreateInfo Rasterizer = {};
	Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	Rasterizer.depthClampEnable = VK_FALSE;
	Rasterizer.rasterizerDiscardEnable = VK_FALSE;
	Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	Rasterizer.lineWidth = 1.0f;
	Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	Rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	Rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo Multisampling = {};
	Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	Multisampling.sampleShadingEnable = VK_FALSE;
	Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState ColorBlendAttachment = {};
	ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo ColorBlending = {};
	ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlending.logicOpEnable = VK_FALSE;
	ColorBlending.logicOp = VK_LOGIC_OP_COPY;
	ColorBlending.attachmentCount = 1;
	ColorBlending.pAttachments = &ColorBlendAttachment;
	ColorBlending.blendConstants[0] = 0.0f;
	ColorBlending.blendConstants[1] = 0.0f;
	ColorBlending.blendConstants[2] = 0.0f;
	ColorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 1;
	PipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;

	VkResult Result = vkCreatePipelineLayout(GPUInfo.Device, &PipelineLayoutInfo, nullptr, &PipelineLayout);
	if (Result != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo PipelineInfo = {};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = 2;
	PipelineInfo.pStages = ShaderStages;
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &InputAssembly;
	PipelineInfo.pViewportState = &ViewportState;
	PipelineInfo.pRasterizationState = &Rasterizer;
	PipelineInfo.pMultisampleState = &Multisampling;
	PipelineInfo.pColorBlendState = &ColorBlending;
	PipelineInfo.layout = PipelineLayout;
	PipelineInfo.renderPass = RenderPass;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	 Result = vkCreateGraphicsPipelines(GPUInfo.Device, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &GraphicsPipeline);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline.");
	}

	vkDestroyShaderModule(GPUInfo.Device, FragmentShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, VertexShaderModule, nullptr);
}

void ValkanGraphics::SetUpFrameBuffers()
{
	SwapChainFramebuffers.resize(SwapChainImageViews.size());

	for (size_t x = 0; x < SwapChainImageViews.size(); x++) {
		VkImageView attachments[] = 
		{
			SwapChainImageViews[x]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = SwapChainExtent.width;
		framebufferInfo.height = SwapChainExtent.height;
		framebufferInfo.layers = 1;

		VkResult Result = vkCreateFramebuffer(GPUInfo.Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[x]);
		if (Result != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

}

void ValkanGraphics::SetUpCommandPool()
{
	VulkanQueueFamily QueueFamilyIndices = FindQueueFamilies(GPUInfo.PhysicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily.value();

	if (vkCreateCommandPool(GPUInfo.Device, &poolInfo, nullptr, &CommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void ValkanGraphics::SetUpTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(GPUInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(GPUInfo.Device, stagingBufferMemory);

	stbi_image_free(pixels);

	CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TextureImage, TextureImageMemory);

	transitionImageLayout(TextureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer, TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	transitionImageLayout(TextureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(GPUInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(GPUInfo.Device, stagingBufferMemory, nullptr);
}

void ValkanGraphics::SetUpTextureImageView()
{
	TextureImageView = CreateImageView(TextureImage, VK_FORMAT_R8G8B8A8_UNORM);
}

void ValkanGraphics::SetUpTextureSampler()
{
	VkSamplerCreateInfo SamplerInfo = {};
	SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.magFilter = VK_FILTER_LINEAR;
	SamplerInfo.minFilter = VK_FILTER_LINEAR;
	SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.anisotropyEnable = VK_FALSE;
	SamplerInfo.maxAnisotropy = 1;
	SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	SamplerInfo.unnormalizedCoordinates = VK_FALSE;
	SamplerInfo.compareEnable = VK_FALSE;
	SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	SamplerInfo.mipLodBias = 0.0f;
	SamplerInfo.minLod = 0.0f;
	SamplerInfo.maxLod = 0.0f;

	VkResult Result = vkCreateSampler(GPUInfo.Device, &SamplerInfo, nullptr, &TextureSampler);
	if (Result != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void ValkanGraphics::SetUpVertexBuffers()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(GPUInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(GPUInfo.Device, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

	CopyBuffer(stagingBuffer, VertexBuffer, bufferSize);

	vkDestroyBuffer(GPUInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(GPUInfo.Device, stagingBufferMemory, nullptr);
}

void ValkanGraphics::SetUpIndexBuffers()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(GPUInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(GPUInfo.Device, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);

	CopyBuffer(stagingBuffer, IndexBuffer, bufferSize);

	vkDestroyBuffer(GPUInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(GPUInfo.Device, stagingBufferMemory, nullptr);
}

void ValkanGraphics::SetUpUniformBuffer()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(SwapChainImages.size());
	uniformBuffersMemory.resize(SwapChainImages.size());

	for (size_t i = 0; i < SwapChainImages.size(); i++) {
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void ValkanGraphics::SetUpDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> PoolSizes = {};
	PoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	PoolSizes[0].descriptorCount = static_cast<unsigned int>(SwapChainImages.size());
	PoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	PoolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChainImages.size());

	VkDescriptorPoolCreateInfo DescriptorPoolInfo = {};
	DescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(PoolSizes.size());
	DescriptorPoolInfo.pPoolSizes = PoolSizes.data();
	DescriptorPoolInfo.maxSets = static_cast<unsigned int>(SwapChainImages.size());

	VkResult Result = vkCreateDescriptorPool(GPUInfo.Device, &DescriptorPoolInfo, nullptr, &DescriptorPool);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool.");
	}
}

void ValkanGraphics::SetUpDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(SwapChainImages.size(), DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	DescriptorSets.resize(SwapChainImages.size());
	if (vkAllocateDescriptorSets(GPUInfo.Device, &allocInfo, DescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < SwapChainImages.size(); i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = TextureImageView;
		imageInfo.sampler = TextureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(GPUInfo.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

uint32_t ValkanGraphics::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(GPUInfo.PhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void ValkanGraphics::SetUpCommandBuffers()
{
	CommandBuffers.resize(SwapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)CommandBuffers.size();

	if (vkAllocateCommandBuffers(GPUInfo.Device, &allocInfo, CommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < CommandBuffers.size(); i++) 
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = RenderPass;
		renderPassInfo.framebuffer = SwapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = SwapChainExtent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		VkBuffer VertexBuffers[] = { VertexBuffer };
		VkDeviceSize Offsets[] = { 0 };

		vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindVertexBuffers(CommandBuffers[i], 0, 1, VertexBuffers, Offsets);
		vkCmdBindIndexBuffer(CommandBuffers[i], IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
		vkCmdBindDescriptorSets(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayout, 0, 1, &DescriptorSets[i], 0, nullptr);
		vkCmdDrawIndexed(CommandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(CommandBuffers[i]);

		if (vkEndCommandBuffer(CommandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void ValkanGraphics::SetUpSyncObjects()
{
	ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	ImagesInFlight.resize(SwapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(GPUInfo.Device, &semaphoreInfo, nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(GPUInfo.Device, &semaphoreInfo, nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(GPUInfo.Device, &fenceInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void ValkanGraphics::CleanUpSwapChain()
{
	for (size_t x = 0; x < SwapChainFramebuffers.size(); x++)
	{
		vkDestroyFramebuffer(GPUInfo.Device, SwapChainFramebuffers[x], nullptr);
	}

	vkFreeCommandBuffers(GPUInfo.Device, CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());
	vkDestroyPipeline(GPUInfo.Device, GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(GPUInfo.Device, PipelineLayout, nullptr);
	vkDestroyRenderPass(GPUInfo.Device, RenderPass, nullptr);

	for (size_t x = 0; x < SwapChainImageViews.size(); x++) 
	{
		vkDestroyImageView(GPUInfo.Device, SwapChainImageViews[x], nullptr);
	}

	vkDestroySwapchainKHR(GPUInfo.Device, SwapChain, nullptr);
}

void ValkanGraphics::RecreateSwapChain()
{
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
	while (width == 0 ||
		   height == 0)
	{
		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(GPUInfo.Device);

	CleanUpSwapChain();

	SetUpSwapChain();
	SetUpImageViews();
	SetUpRenderPass();
	SetUpGraphicsPipeLine();
	SetUpFrameBuffers();
	SetUpUniformBuffer();
	SetUpDescriptorPool();
	SetUpDescriptorSets();
	SetUpCommandBuffers();
}

void ValkanGraphics::DrawFrame() {
	vkWaitForFences(GPUInfo.Device, 1, &InFlightFences[CurrentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	VkResult Result = vkAcquireNextImageKHR(GPUInfo.Device, SwapChain, UINT64_MAX, ImageAvailableSemaphores[CurrentFrame], VK_NULL_HANDLE, &imageIndex);
	if (Result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (Result != VK_SUCCESS &&
			 Result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image.");
	}

	if (ImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(GPUInfo.Device, 1, &ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	UpdateUniformBuffer(imageIndex);

	ImagesInFlight[imageIndex] = InFlightFences[CurrentFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { ImageAvailableSemaphores[CurrentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { RenderFinishedSemaphores[CurrentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(GPUInfo.Device, 1, &InFlightFences[CurrentFrame]);

	if (vkQueueSubmit(GraphicsQueue, 1, &submitInfo, InFlightFences[CurrentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { SwapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	Result = vkQueuePresentKHR(PresentQueue, &presentInfo);
	if (Result == VK_ERROR_OUT_OF_DATE_KHR || 
		Result == VK_SUBOPTIMAL_KHR ||
		Window.GetFrameBufferResizedFlag()) 
	{
		Window.SetFrameBufferResizedFlag(false);
		RecreateSwapChain();
	}
	else if (Result != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to present swap chain image.");
	}

	CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkImageView ValkanGraphics::CreateImageView(VkImage image, VkFormat format)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(GPUInfo.Device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void ValkanGraphics::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(GPUInfo.Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(GPUInfo.Device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(GPUInfo.Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(GPUInfo.Device, buffer, bufferMemory, 0);
}

VkCommandBuffer ValkanGraphics::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(GPUInfo.Device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void ValkanGraphics::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(GraphicsQueue);

	vkFreeCommandBuffers(GPUInfo.Device, CommandPool, 1, &commandBuffer);
}

void ValkanGraphics::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

void ValkanGraphics::UpdateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), SwapChainExtent.width / (float)SwapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(GPUInfo.Device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(GPUInfo.Device, uniformBuffersMemory[currentImage]);
}

void ValkanGraphics::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(GPUInfo.Device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(GPUInfo.Device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(GPUInfo.Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(GPUInfo.Device, image, imageMemory, 0);
}

void ValkanGraphics::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	endSingleTimeCommands(commandBuffer);
}

void ValkanGraphics::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endSingleTimeCommands(commandBuffer);
}

VulkanQueueFamily ValkanGraphics::FindQueueFamilies(VkPhysicalDevice physicalDevice)
{
	VulkanQueueFamily Indices;

	unsigned int QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, QueueFamilies.data());

	for (int x = 0; x <= QueueFamilies.size() - 1; x++)
	{
		if (QueueFamilies[x].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			Indices.GraphicsFamily = x;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, x, VulkanSurface, &presentSupport);

		if (presentSupport) 
		{
			Indices.PresentationFamily = x;
		}

		if (Indices.IsComplete()) {
			break;
		}
	}

	return Indices;
}

bool ValkanGraphics::ValidationLayerSupport()
{
	unsigned int LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

	std::vector<VkLayerProperties> AvailableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	for (const char* LayerName : ValidationLayers)
	{
		bool LayerFound = false;

		for (const auto& LayerProperties : AvailableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				LayerFound = true;
				break;
			}
		}

		if (!LayerFound)
		{
			return false;
		}
	}

	return true;
}

bool ValkanGraphics::IsDeviceCompatable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties DeviceProperties;
	VkPhysicalDeviceFeatures DeviceFeatures;

	vkGetPhysicalDeviceProperties(device, &DeviceProperties);
	vkGetPhysicalDeviceFeatures(device, &DeviceFeatures);

	VulkanQueueFamily Indices = FindQueueFamilies(device);

	bool ExtensionSupported = CheckDeviceExtensionSupport(device);

	bool SwapChainAdequate = false;
	if (ExtensionSupported) 
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		SwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return Indices.IsComplete() && 
		   ExtensionSupported &&
		   SwapChainAdequate &&
		   DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		   DeviceFeatures.geometryShader &&
		   DeviceFeatures.samplerAnisotropy;
}

bool ValkanGraphics::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	unsigned int ExtensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, nullptr);

	std::vector<VkExtensionProperties> AvailableExtension(ExtensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, AvailableExtension.data());

	std::set<std::string> RequiredExtension(DeviceExtensions.begin(), DeviceExtensions.end());

	for (const auto& extension : AvailableExtension)
	{
		RequiredExtension.erase(extension.extensionName);
	}

	return RequiredExtension.empty();
}

SwapChainSupportDetails ValkanGraphics::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails Details;
	unsigned int FormatCount;
	unsigned int PresentModeCount;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanSurface, &Details.capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface, &FormatCount, Details.formats.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());

	if (PresentModeCount != 0)
	{
		Details.presentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface, &PresentModeCount, Details.presentModes.data());
	}

	return Details;
}

VkSurfaceFormatKHR ValkanGraphics::SetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	for (const auto& availableFormat : AvailableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return AvailableFormats[0];
}

VkPresentModeKHR ValkanGraphics::SetSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
{
	for (const auto& availablePresentMode : AvailablePresentModes) 
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ValkanGraphics::SetSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{
	if (Capabilities.currentExtent.width != UINT32_MAX) {
		return Capabilities.currentExtent;
	}
	else 
	{
		int width = Window.GetWindowWidth();
		int height = Window.GetWindowHeight();

		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		VkExtent2D ActualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
		ActualExtent.height = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

		return ActualExtent;
	}
}

std::vector<const char*> ValkanGraphics::GetRequiredExtensions()
{
	unsigned int glfwExtenstionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtenstionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtenstionCount);

	if (EnableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void ValkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		DrawFrame();
	}

	vkDeviceWaitIdle(GPUInfo.Device);
}

void ValkanGraphics::Run()
{
	MainLoop();
}

