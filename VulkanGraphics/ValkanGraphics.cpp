#include "ValkanGraphics.h"
#include <set>
#include <algorithm> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



ValkanGraphics::ValkanGraphics(unsigned int width, unsigned int height, const char* windowName) : EnableValidationLayers(true)
{
	ValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
	DeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	Window = VulkanWindow(width, height, windowName);
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.3f));

	SetUpVulkanInstance();
	SetUpDebugger();
	SetUpSurface();
	SetUpPhysicalDevice();
	SetUpLogicalDevice();
	SetUpSwapChain();
	SetUpImageViews();
	SetUpRenderPass();
	SetDescriptorSetLayout();
	SetSkyBoxDescriptorSetLayout();
	SetUpDepthBuffer();
	SetUpFrameBuffers();
	SetUpCommandPool();
	SetUpGraphicsPipeLine();
	SetUpMeshObjects();
	SetUpCommandBuffers();
	SetUpSyncObjects();
}

ValkanGraphics::~ValkanGraphics()
{
	CleanUpSwapChain();

	for (Mesh mesh : MeshObject)
	{
		mesh.Destory(GPUInfo.Device, SwapChainImages.size());
	}

	for (Mesh mesh : LightMeshObject)
	{
		mesh.Destory(GPUInfo.Device, SwapChainImages.size());
	}

	vkDestroyDescriptorSetLayout(GPUInfo.Device, DescriptorSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(GPUInfo.Device, SkyBoxDescriptorSetLayout, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
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
	deviceFeatures.fillModeNonSolid = VK_TRUE;

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
	for (size_t i = 0; i < SwapChainImages.size(); i++) 
	{
		SwapChainImageViews[i] = CreateImageView(SwapChainImages[i], SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
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

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
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

	VkDescriptorSetLayoutBinding SamplerLayoutBinding2 = {};
	SamplerLayoutBinding2.binding = 2;
	SamplerLayoutBinding2.descriptorCount = 1;
	SamplerLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	SamplerLayoutBinding2.pImmutableSamplers = nullptr;
	SamplerLayoutBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding AmbiantLayoutBinding = {};
	AmbiantLayoutBinding.binding = 3;
	AmbiantLayoutBinding.descriptorCount = 1;
	AmbiantLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	AmbiantLayoutBinding.pImmutableSamplers = nullptr;
	AmbiantLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 4> bindings = { UBOLayoutBinding, SamplerLayoutBinding, SamplerLayoutBinding2, AmbiantLayoutBinding };
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

void ValkanGraphics::SetSkyBoxDescriptorSetLayout()
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

	VkResult Result = vkCreateDescriptorSetLayout(GPUInfo.Device, &LayoutInfo, nullptr, &SkyBoxDescriptorSetLayout);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout.");
	}
}

void ValkanGraphics::SetUpGraphicsPipeLine()
{
	VkShaderModule VertexShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/vert.spv");
	VkShaderModule FragmentShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/frag.spv");
	VkShaderModule LightVertexShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/lightvert.spv");
	VkShaderModule LightFragmentShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/lightfrag.spv");
	VkShaderModule SkyBoxVertexShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/SkyBoxVert.spv");
	VkShaderModule SkyBoxFragmentShaderModule = CompileShader.CompileShader(GPUInfo.Device, "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/Shaders/SkyBoxFrag.spv");

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();
	auto SkyBoxBindingDescription = CubeMapVertex::GetBindingDescription();
	auto SkyBoxAttributeDescriptions = CubeMapVertex::GetAttributeDescriptions();

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

	VkPipelineShaderStageCreateInfo lightVertexShaderStageInfo = {};
	lightVertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	lightVertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	lightVertexShaderStageInfo.module = LightVertexShaderModule;
	lightVertexShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo lightFragmentShaderStageInfo = {};
	lightFragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	lightFragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	lightFragmentShaderStageInfo.module = LightFragmentShaderModule;
	lightFragmentShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxVertexShaderStageInfo = {};
	SkyBoxVertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxVertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	SkyBoxVertexShaderStageInfo.module = SkyBoxVertexShaderModule;
	SkyBoxVertexShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxFragmentShaderStageInfo = {};
	SkyBoxFragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxFragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	SkyBoxFragmentShaderStageInfo.module = SkyBoxFragmentShaderModule;
	SkyBoxFragmentShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexShaderStageInfo, FragmentShaderStageInfo };
	VkPipelineShaderStageCreateInfo LightShaderStages[] = { lightVertexShaderStageInfo, lightFragmentShaderStageInfo };
	VkPipelineShaderStageCreateInfo SkyBoxShaderStages[] = { SkyBoxVertexShaderStageInfo, SkyBoxFragmentShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.vertexBindingDescriptionCount = 1;
	VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	VertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	VertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineVertexInputStateCreateInfo SkyBoxVertexInputInfo = {};
	SkyBoxVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	SkyBoxVertexInputInfo.vertexBindingDescriptionCount = 1;
	SkyBoxVertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(SkyBoxAttributeDescriptions.size());
	SkyBoxVertexInputInfo.pVertexBindingDescriptions = &SkyBoxBindingDescription;
	SkyBoxVertexInputInfo.pVertexAttributeDescriptions = SkyBoxAttributeDescriptions.data();

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
	Rasterizer.cullMode = VK_CULL_MODE_NONE;
	Rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	Rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineRasterizationStateCreateInfo LineRasterizer = {};
	LineRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	LineRasterizer.depthClampEnable = VK_FALSE;
	LineRasterizer.rasterizerDiscardEnable = VK_FALSE;
	LineRasterizer.polygonMode = VK_POLYGON_MODE_LINE;
	LineRasterizer.lineWidth = 1.0f;
	LineRasterizer.cullMode = VK_CULL_MODE_NONE;
	LineRasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	LineRasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineRasterizationStateCreateInfo VertexRasterizer = {};
	VertexRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	VertexRasterizer.depthClampEnable = VK_FALSE;
	VertexRasterizer.rasterizerDiscardEnable = VK_FALSE;
	VertexRasterizer.polygonMode = VK_POLYGON_MODE_POINT;
	VertexRasterizer.lineWidth = 1.0f;
	VertexRasterizer.cullMode = VK_CULL_MODE_NONE;
	VertexRasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VertexRasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo Multisampling = {};
	Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	Multisampling.sampleShadingEnable = VK_FALSE;
	Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo DepthStencil = {};
	DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencil.depthTestEnable = VK_TRUE;
	DepthStencil.depthWriteEnable = VK_TRUE;
	DepthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	DepthStencil.depthBoundsTestEnable = VK_FALSE;
	DepthStencil.stencilTestEnable = VK_FALSE;

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

	VkPipelineLayoutCreateInfo SkyBoxPipelineLayoutInfo = {};
	SkyBoxPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	SkyBoxPipelineLayoutInfo.setLayoutCount = 1;
	SkyBoxPipelineLayoutInfo.pSetLayouts = &SkyBoxDescriptorSetLayout;

	Result = vkCreatePipelineLayout(GPUInfo.Device, &SkyBoxPipelineLayoutInfo, nullptr, &SkyBoxPipelineLayout);
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
	PipelineInfo.pDepthStencilState = &DepthStencil;
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

	VkGraphicsPipelineCreateInfo SkyBoxPipelineInfo = {};
	SkyBoxPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	SkyBoxPipelineInfo.stageCount = 2;
	SkyBoxPipelineInfo.pStages = SkyBoxShaderStages;
	SkyBoxPipelineInfo.pVertexInputState = &SkyBoxVertexInputInfo;
	SkyBoxPipelineInfo.pInputAssemblyState = &InputAssembly;
	SkyBoxPipelineInfo.pViewportState = &ViewportState;
	SkyBoxPipelineInfo.pRasterizationState = &Rasterizer;
	SkyBoxPipelineInfo.pMultisampleState = &Multisampling;
	SkyBoxPipelineInfo.pDepthStencilState = &DepthStencil;
	SkyBoxPipelineInfo.pColorBlendState = &ColorBlending;
	SkyBoxPipelineInfo.layout = SkyBoxPipelineLayout;
	SkyBoxPipelineInfo.renderPass = RenderPass;
	SkyBoxPipelineInfo.subpass = 0;
	SkyBoxPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	Result = vkCreateGraphicsPipelines(GPUInfo.Device, VK_NULL_HANDLE, 1, &SkyBoxPipelineInfo, nullptr, &SkyBoxPipeline);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline.");
	}

	VkGraphicsPipelineCreateInfo LightPipelineInfo = {};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = 2;
	PipelineInfo.pStages = LightShaderStages;
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

	Result = vkCreateGraphicsPipelines(GPUInfo.Device, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &LightGraphicsPipeline);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline.");
	}

	VkGraphicsPipelineCreateInfo VertexeShaderPipelineInfo = {};
	VertexeShaderPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	VertexeShaderPipelineInfo.stageCount = 2;
	VertexeShaderPipelineInfo.pStages = LightShaderStages;
	VertexeShaderPipelineInfo.pVertexInputState = &VertexInputInfo;
	VertexeShaderPipelineInfo.pInputAssemblyState = &InputAssembly;
	VertexeShaderPipelineInfo.pViewportState = &ViewportState;
	VertexeShaderPipelineInfo.pRasterizationState = &VertexRasterizer;
	VertexeShaderPipelineInfo.pMultisampleState = &Multisampling;
	VertexeShaderPipelineInfo.pDepthStencilState = &DepthStencil;
	VertexeShaderPipelineInfo.pColorBlendState = &ColorBlending;
	VertexeShaderPipelineInfo.layout = PipelineLayout;
	VertexeShaderPipelineInfo.renderPass = RenderPass;
	VertexeShaderPipelineInfo.subpass = 0;
	VertexeShaderPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	Result = vkCreateGraphicsPipelines(GPUInfo.Device, VK_NULL_HANDLE, 1, &VertexeShaderPipelineInfo, nullptr, &VertexShaderPipeline);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline.");
	}

	vkDestroyShaderModule(GPUInfo.Device, FragmentShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, VertexShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, LightFragmentShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, LightVertexShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, SkyBoxFragmentShaderModule, nullptr);
	vkDestroyShaderModule(GPUInfo.Device, SkyBoxVertexShaderModule, nullptr);
}

void ValkanGraphics::SetUpFrameBuffers()
{
	SwapChainFramebuffers.resize(SwapChainImageViews.size());

	for (size_t x = 0; x < SwapChainImageViews.size(); x++) 
	{
		std::array<VkImageView, 2> attachments = { SwapChainImageViews[x], DepthImageView };

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
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

void ValkanGraphics::SetUpDepthBuffer()
{
	VkFormat DepthFormat = findDepthFormat();
	CreateImage(SwapChainExtent.width, SwapChainExtent.height, DepthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, DepthImage, DepthImageMemory);
	DepthImageView = CreateImageView(DepthImage, DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void ValkanGraphics::SetUpMeshObjects()
{
	std::vector<std::string> CubeOfMap;
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/back.jpg");
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/bottom.jpg");
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/front.jpg");
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/left.jpg");
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/right.jpg");
	CubeOfMap.emplace_back("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/top.jpg");

	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
	MeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));

	LightMeshObject.emplace_back(Mesh("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/texture.jpg", "C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png", SwapChainImages.size(), GPUInfo.Device, GPUInfo.PhysicalDevice, CommandBuffers, vertices, indices, CommandPool, GraphicsQueue, DescriptorSetLayout));
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
		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

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
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);



		for (Mesh mesh : MeshObject)
		{
			if (FillMode == PolygonFillMode::GPX_FILL_SOLID)
			{
				mesh.Draw(CommandBuffers[i], GraphicsPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
			else if (FillMode == PolygonFillMode::GPX_FILL_LINE)
			{
				mesh.Draw(CommandBuffers[i], LineShaderPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
			else if (FillMode == PolygonFillMode::GPX_FILL_VERTEX)
			{
				mesh.Draw(CommandBuffers[i], VertexShaderPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
		}

		for (Mesh mesh : LightMeshObject)
		{
			if (FillMode == PolygonFillMode::GPX_FILL_SOLID)
			{
				mesh.Draw(CommandBuffers[i], LightGraphicsPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
			else if (FillMode == PolygonFillMode::GPX_FILL_LINE)
			{
				mesh.Draw(CommandBuffers[i], LineShaderPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
			else if (FillMode == PolygonFillMode::GPX_FILL_VERTEX)
			{
				mesh.Draw(CommandBuffers[i], VertexShaderPipeline, PipelineLayout, static_cast<uint32_t>(indices.size()), i);
			}
		}

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
	vkDestroyImageView(GPUInfo.Device, DepthImageView, nullptr);
	vkDestroyImage(GPUInfo.Device, DepthImage, nullptr);
	vkFreeMemory(GPUInfo.Device, DepthImageMemory, nullptr);

	for (size_t x = 0; x < SwapChainFramebuffers.size(); x++)
	{
		vkDestroyFramebuffer(GPUInfo.Device, SwapChainFramebuffers[x], nullptr);
	}

	vkFreeCommandBuffers(GPUInfo.Device, CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());

	vkDestroyPipeline(GPUInfo.Device, GraphicsPipeline, nullptr);
	vkDestroyPipeline(GPUInfo.Device, LightGraphicsPipeline, nullptr);
	vkDestroyPipeline(GPUInfo.Device, SkyBoxPipeline, nullptr);
	vkDestroyPipeline(GPUInfo.Device, LineShaderPipeline, nullptr);
	vkDestroyPipeline(GPUInfo.Device, VertexShaderPipeline, nullptr);

	vkDestroyPipelineLayout(GPUInfo.Device, PipelineLayout, nullptr);
	vkDestroyPipelineLayout(GPUInfo.Device, SkyBoxPipelineLayout, nullptr);

	vkDestroyRenderPass(GPUInfo.Device, RenderPass, nullptr);

	for (size_t x = 0; x < SwapChainImageViews.size(); x++) 
	{
		vkDestroyImageView(GPUInfo.Device, SwapChainImageViews[x], nullptr);
	}

	vkDestroySwapchainKHR(GPUInfo.Device, SwapChain, nullptr);

	for (Mesh mesh : MeshObject)
	{
		mesh.DestoryBufferObjects(GPUInfo.Device, SwapChainFramebuffers.size());
	}

	for (Mesh mesh : LightMeshObject)
	{
		mesh.DestoryBufferObjects(GPUInfo.Device, SwapChainFramebuffers.size());
	}
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
	SetUpDepthBuffer();
	SetUpFrameBuffers();

	//for (Mesh mesh : MeshObject)
	//{
	//	mesh.SetUpDescriptorPool(SwapChainImages.size(), GPUInfo.Device);
	//	mesh.SetUpDescriptorSets(SwapChainImages.size(), GPUInfo.Device, DescriptorSetLayout);
	//}

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

	VkSemaphore waitSemaphores[] = { ImageAvailableSemaphores[CurrentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { RenderFinishedSemaphores[CurrentFrame] };

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(GPUInfo.Device, 1, &InFlightFences[CurrentFrame]);

	if (vkQueueSubmit(GraphicsQueue, 1, &submitInfo, InFlightFences[CurrentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkSwapchainKHR swapChains[] = { SwapChain };

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
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

void ValkanGraphics::CreatePipeLine()
{
}

VkImageView ValkanGraphics::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
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

void ValkanGraphics::UpdateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), (float)SwapChainExtent.width / (float)SwapChainExtent.height, 0.1f, 100.0f);

	UniformBufferObject2 ubo = {};
	ubo.view = view;
	ubo.proj = projection;
	ubo.proj[1][1] *= -1;

	glm::vec3 cubePositions[] = 
	{
	   glm::vec3(0.0f,  0.0f,  0.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
		ubo.model = model;

		LightingStruct light = {};
		light.Ambient = glm::vec3(1.0f, sin(time), 0.0f);
		light.Diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
		light.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		light.Specular = glm::vec3(0.0f, 0.0f, 1.0f);

		MeshObject[i].UpdateUniformBuffers(ubo, light, currentImage);
	}

	for (Mesh mesh : LightMeshObject)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.7f, 3.0f, -7.5f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		ubo.model = model;

		LightingStruct light = {};
		light.Ambient = glm::vec3(1.0f, sin(time), 0.0f);
		light.Diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
		light.Position = glm::vec3(0.0f, 1.0f, 0.0f);
		light.Specular = glm::vec3(0.0f, 0.0f, 1.0f);

		mesh.UpdateUniformBuffers(ubo, light, currentImage);
	}
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

VkFormat ValkanGraphics::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(GPUInfo.PhysicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat ValkanGraphics::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool ValkanGraphics::hasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
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

void ValkanGraphics::UpdateMouse()
{
	if (glfwGetMouseButton(Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwGetCursorPos(Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
		if (firstMouse)
		{
			lastX = MouseXPos;
			lastY = MouseYPos;
			firstMouse = false;
		}

		float xoffset = MouseXPos - lastX;
		float yoffset = lastY - MouseYPos;

		lastX = MouseXPos;
		lastY = MouseYPos;

		camera.UpdateMouse(xoffset, yoffset);
	}
	else
	{
		firstMouse = true;
	}
}

void ValkanGraphics::UpdateKeyboard()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
		camera.UpdateKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
		camera.UpdateKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
		camera.UpdateKeyboard(LEFT, deltaTime);
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
		camera.UpdateKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_5) == GLFW_PRESS)
	{
		FillMode = PolygonFillMode::GPX_FILL_SOLID;
		vkDeviceWaitIdle(GPUInfo.Device);
		CommandBuffers.clear();
		SetUpCommandBuffers();
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_6) == GLFW_PRESS)
	{
		FillMode = PolygonFillMode::GPX_FILL_LINE;
		vkDeviceWaitIdle(GPUInfo.Device);
		CommandBuffers.clear();
		SetUpCommandBuffers();
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_7) == GLFW_PRESS)
	{
		FillMode = PolygonFillMode::GPX_FILL_VERTEX;
		vkDeviceWaitIdle(GPUInfo.Device);
		CommandBuffers.clear();
		SetUpCommandBuffers();
	}
}

void ValkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		DrawFrame();
		UpdateMouse();
		UpdateKeyboard();
	}

	vkDeviceWaitIdle(GPUInfo.Device);
}

void ValkanGraphics::Run()
{
	MainLoop();
}

