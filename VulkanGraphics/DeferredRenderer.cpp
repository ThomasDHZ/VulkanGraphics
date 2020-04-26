//#include "DeferredRenderer.h"
//#include <set>
//
//DeferredRenderer::DeferredRenderer() : BaseRenderer()
//{
//}
//
//DeferredRenderer::DeferredRenderer(std::vector<Mesh>* meshList, std::vector<Model>* modelList, SkyBox* skybox, VkInstance instance, GLFWwindow* window) : BaseRenderer(meshList, modelList, skybox, instance, window)
//{
//	createSurface(instance);
//	pickPhysicalDevice(instance);
//	createLogicalDevice();
//	createSwapChain();
//	createImageViews();
//	createRenderPass();
//	createDescriptorSetLayout();
//	createGraphicsPipeline();
//	createCommandPool();
//	//PositionAttachment = InputAttachment(UpdateDeviceInfo(), AttachmentType::VkPositionAttachment, swapChainExtent.width, swapChainExtent.height);
//	//NormalAttachment = InputAttachment(UpdateDeviceInfo(), AttachmentType::VkNormalAttachment, swapChainExtent.width, swapChainExtent.height);
//	//AlbedoAttachment = InputAttachment(UpdateDeviceInfo(), AttachmentType::VkAlbedoAttachment, swapChainExtent.width, swapChainExtent.height);
//	//DepthAttachment = InputAttachment(UpdateDeviceInfo(), AttachmentType::VkDepthAttachemnt, swapChainExtent.width, swapChainExtent.height);
//	createFramebuffers();
//}
//
//DeferredRenderer::~DeferredRenderer()
//{
//}
//
//void DeferredRenderer::createRenderPass() 
//{
//	VkAttachmentDescription ColorAttachment = {};
//	ColorAttachment.format = swapChainImageFormat;
//	ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//	ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//	VkAttachmentDescription PositionAttachment = {};
//	PositionAttachment.format = VK_FORMAT_R16G16B16A16_SFLOAT;
//	PositionAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	PositionAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	PositionAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	PositionAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	PositionAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	PositionAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	PositionAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription NormalAttachment = {};
//	NormalAttachment.format = VK_FORMAT_R16G16B16A16_SFLOAT;
//	NormalAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	NormalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	NormalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	NormalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	NormalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	NormalAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	NormalAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription AlbedoAttachment = {};
//	AlbedoAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
//	AlbedoAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	AlbedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	AlbedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	AlbedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	AlbedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	AlbedoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	AlbedoAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription DepthAttachment = {};
//	DepthAttachment.format = findDepthFormat();
//	DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentReference colorReferences[4];
//	colorReferences[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[1] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[2] = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	colorReferences[3] = { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//	VkAttachmentReference depthReference = { 4, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
//
//	VkSubpassDescription GBufferPass;
//	GBufferPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	GBufferPass.colorAttachmentCount = 4;
//	GBufferPass.pColorAttachments = colorReferences;
//	GBufferPass.pDepthStencilAttachment = &depthReference;
//
//	VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//
//	VkAttachmentReference inputReferences[3];
//	inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//	inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//	inputReferences[2] = { 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
//
//	uint32_t preserveAttachmentIndex = 1;
//
//	VkSubpassDescription MainRenderPass;
//	MainRenderPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	MainRenderPass.colorAttachmentCount = 1;
//	MainRenderPass.pColorAttachments = &colorReference;
//	MainRenderPass.pDepthStencilAttachment = &depthReference;
//	MainRenderPass.inputAttachmentCount = 3;
//	MainRenderPass.pInputAttachments = inputReferences;
//
//	
//	VkSubpassDependency dependice1;
//	dependice1.srcSubpass = VK_SUBPASS_EXTERNAL;
//	dependice1.dstSubpass = 0;
//	dependice1.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//	dependice1.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependice1.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//	dependice1.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependice1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	VkSubpassDependency dependice2;
//	dependice2.srcSubpass = 0;
//	dependice2.dstSubpass = 1;
//	dependice2.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependice2.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//	dependice2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependice2.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//	dependice2.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	VkSubpassDependency dependice3;
//	dependice3.srcSubpass = 0;
//	dependice3.dstSubpass = VK_SUBPASS_EXTERNAL;
//	dependice3.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependice3.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//	dependice3.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	dependice3.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//	dependice3.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//	std::vector<VkSubpassDescription> SubPassList = { GBufferPass, MainRenderPass };
//	std::vector<VkSubpassDependency> dependencies = { dependice1, dependice2, dependice3 };
//	std::vector<VkAttachmentDescription> attachments = { ColorAttachment, PositionAttachment, NormalAttachment, AlbedoAttachment, DepthAttachment };
//	//BaseRenderer::createRenderPass(SubPassList, dependencies, attachments);
//}
//
//
//void DeferredRenderer::createDescriptorSetLayout()
//{
//
//	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
//	uboLayoutBinding.binding = 0;
//	uboLayoutBinding.descriptorCount = 1;
//	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	uboLayoutBinding.pImmutableSamplers = nullptr;
//	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//
//	VkDescriptorSetLayoutBinding DiffuseMapBinding = {};
//	DiffuseMapBinding.binding = 1;
//	DiffuseMapBinding.descriptorCount = 1;
//	DiffuseMapBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//	DiffuseMapBinding.pImmutableSamplers = nullptr;
//	DiffuseMapBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	VkDescriptorSetLayoutBinding SpecularMapBinding = {};
//	SpecularMapBinding.binding = 2;
//	SpecularMapBinding.descriptorCount = 1;
//	SpecularMapBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//	SpecularMapBinding.pImmutableSamplers = nullptr;
//	SpecularMapBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	std::vector<VkDescriptorSetLayoutBinding> bindings = { uboLayoutBinding, DiffuseMapBinding, SpecularMapBinding };
//	//BaseRenderer::createDescriptorSetLayout(bindings, graphics);
//}
//
//void DeferredRenderer::createGraphicsPipeline() {
//	auto vertShaderCode = readFile("Shaders/vert.spv");
//	auto fragShaderCode = readFile("Shaders/frag.spv");
//
//	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
//	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
//
//	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
//	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//	vertShaderStageInfo.module = vertShaderModule;
//	vertShaderStageInfo.pName = "main";
//
//	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
//	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//	fragShaderStageInfo.module = fragShaderModule;
//	fragShaderStageInfo.pName = "main";
//
//	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
//
//	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
//	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//
//	auto bindingDescription = Vertex::GetBindingDescription();
//	auto attributeDescriptions = Vertex::GetAttributeDescriptions();
//
//	vertexInputInfo.vertexBindingDescriptionCount = 1;
//	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
//	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
//	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
//
//	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
//	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//	inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//	VkViewport viewport = {};
//	viewport.x = 0.0f;
//	viewport.y = 0.0f;
//	viewport.width = (float)swapChainExtent.width;
//	viewport.height = (float)swapChainExtent.height;
//	viewport.minDepth = 0.0f;
//	viewport.maxDepth = 1.0f;
//
//	VkRect2D scissor = {};
//	scissor.offset = { 0, 0 };
//	scissor.extent = swapChainExtent;
//
//	VkPipelineViewportStateCreateInfo viewportState = {};
//	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//	viewportState.viewportCount = 1;
//	viewportState.pViewports = &viewport;
//	viewportState.scissorCount = 1;
//	viewportState.pScissors = &scissor;
//
//	VkPipelineRasterizationStateCreateInfo rasterizer = {};
//	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//	rasterizer.depthClampEnable = VK_FALSE;
//	rasterizer.rasterizerDiscardEnable = VK_FALSE;
//	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//	rasterizer.lineWidth = 1.0f;
//	rasterizer.cullMode = VK_CULL_MODE_NONE;
//	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
//	rasterizer.depthBiasEnable = VK_FALSE;
//
//	VkPipelineMultisampleStateCreateInfo multisampling = {};
//	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//	multisampling.sampleShadingEnable = VK_FALSE;
//	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//
//	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
//	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//	depthStencil.depthTestEnable = VK_TRUE;
//	depthStencil.depthWriteEnable = VK_TRUE;
//	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
//	depthStencil.depthBoundsTestEnable = VK_FALSE;
//	depthStencil.stencilTestEnable = VK_FALSE;
//
//
//	std::array<VkPipelineColorBlendAttachmentState, 4> Attachments{};
//	Attachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//	Attachments[0].blendEnable = VK_FALSE;
//
//	Attachments[1].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//	Attachments[1].blendEnable = VK_FALSE;
//
//	Attachments[2].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//	Attachments[2].blendEnable = VK_FALSE;
//
//	Attachments[3].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//	Attachments[3].blendEnable = VK_FALSE;
//
//	VkPipelineColorBlendStateCreateInfo colorBlending = {};
//	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//	colorBlending.logicOpEnable = VK_FALSE;
//	colorBlending.logicOp = VK_LOGIC_OP_COPY;
//	colorBlending.attachmentCount = static_cast<uint32_t>(Attachments.size());
//	colorBlending.pAttachments = Attachments.data();
//	colorBlending.blendConstants[0] = 0.0f;
//	colorBlending.blendConstants[1] = 0.0f;
//	colorBlending.blendConstants[2] = 0.0f;
//	colorBlending.blendConstants[3] = 0.0f;
//
//	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
//	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//	pipelineLayoutInfo.setLayoutCount = 1;
////	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
//
//	//if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
//	//	throw std::runtime_error("failed to create pipeline layout!");
//	//}
//
//	//VkGraphicsPipelineCreateInfo pipelineInfo = {};
//	//pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//	//pipelineInfo.stageCount = 2;
//	//pipelineInfo.pStages = shaderStages;
//	//pipelineInfo.pVertexInputState = &vertexInputInfo;
//	//pipelineInfo.pInputAssemblyState = &inputAssembly;
//	//pipelineInfo.pViewportState = &viewportState;
//	//pipelineInfo.pRasterizationState = &rasterizer;
//	//pipelineInfo.pMultisampleState = &multisampling;
//	//pipelineInfo.pDepthStencilState = &depthStencil;
//	//pipelineInfo.pColorBlendState = &colorBlending;
//	//pipelineInfo.layout = pipelineLayout;
//	//pipelineInfo.renderPass = renderPass;
//	//pipelineInfo.subpass = 0;
//	//pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//
//	//if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
//	//	throw std::runtime_error("failed to create graphics pipeline!");
//	//}
//
//	vkDestroyShaderModule(device, fragShaderModule, nullptr);
//	vkDestroyShaderModule(device, vertShaderModule, nullptr);
//}
//
//void DeferredRenderer::createFramebuffers() {
//	swapChainFramebuffers.resize(swapChainImageViews.size());
//
//	for (size_t i = 0; i < swapChainImageViews.size(); i++)
//	{
//		std::vector<VkImageView> attachments = {
//			swapChainImageViews[i],
//			DepthAttachment.AttachmentImageView
//		};
//
//		BaseRenderer::createFramebuffers(attachments, i);
//	}
//}
//
//void DeferredRenderer::createCommandBuffers()
//{
//	//std::vector<VkClearValue> clearValues = {};
//	//clearValues.resize(2);
//
//	//clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//	//clearValues[1].depthStencil = { 1.0f, 0 };
//
//	//BaseRenderer::createCommandBuffers(clearValues);
//}
//
//void DeferredRenderer::UpdateSwapChain()
//{
//	createSwapChain();
//	createImageViews();
//	createRenderPass();
//	createGraphicsPipeline();
//	PositionAttachment.ReCreateAttachment(AttachmentType::VkPositionAttachment, swapChainExtent.width, swapChainExtent.height);
//	NormalAttachment.ReCreateAttachment(AttachmentType::VkNormalAttachment, swapChainExtent.width, swapChainExtent.height);
//	AlbedoAttachment.ReCreateAttachment(AttachmentType::VkAlbedoAttachment, swapChainExtent.width, swapChainExtent.height);
//	DepthAttachment.ReCreateAttachment(AttachmentType::VkDepthAttachemnt, swapChainExtent.width, swapChainExtent.height);
//	createFramebuffers();
//}
