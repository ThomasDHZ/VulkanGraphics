#include "FrameBufferShader.h"
#include "Mesh.h"
FrameBufferShader::FrameBufferShader() : BaseShader()
{
}

FrameBufferShader::FrameBufferShader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView) : BaseShader(deviceInfo)
{
	CreateDescriptorSetLayout();
	RecreateSwapChainInfo(swapChainExtent, renderPass, PositionImageView, NormalImageView, AlbedoImageView, DepthImageView);
}

FrameBufferShader::~FrameBufferShader()
{
}

void FrameBufferShader::RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView)
{
	CreateShaderPipeLine(swapChainExtent, renderPass);
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(PositionImageView, NormalImageView, AlbedoImageView, DepthImageView);
}

void FrameBufferShader::CreateDescriptorSetLayout()
{
	std::array<DescriptorSetLayoutBindingInfo, 4> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[2].Binding = 2;
	LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[3].Binding = 3;
	LayoutBindingInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	LayoutBindingInfo[3].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	BaseShader::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void FrameBufferShader::CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass)
{
	auto FrameBufferVertShaderCode = ReadShaderFile("shaders/FrameBufferVert.spv");
	auto FrameBufferFrageShaderCode = ReadShaderFile("shaders/FrameBufferFrag.spv");

	VkShaderModule FrameBufferVertShaderModule = CreateShaderModule(FrameBufferVertShaderCode);
	VkShaderModule FrameBufferFragShaderModule = CreateShaderModule(FrameBufferFrageShaderCode);

	VkPipelineShaderStageCreateInfo FrameBufferVertShaderStageInfo = {};
	FrameBufferVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FrameBufferVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	FrameBufferVertShaderStageInfo.module = FrameBufferVertShaderModule;
	FrameBufferVertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FrameBufferFragShaderStageInfo = {};
	FrameBufferFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FrameBufferFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FrameBufferFragShaderStageInfo.module = FrameBufferFragShaderModule;
	FrameBufferFragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FrameBufferShaderStages[] = { FrameBufferVertShaderStageInfo, FrameBufferFragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineVertexInputStateCreateInfo emptyVertexInputStateCI{};
	emptyVertexInputStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	VkPipelineLayoutCreateInfo FrameBufferPipelineLayoutInfo = {};
	FrameBufferPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	FrameBufferPipelineLayoutInfo.setLayoutCount = 1;
	FrameBufferPipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(DeviceInfo.Device, &FrameBufferPipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo FrameBufferPipelineInfo = {};
	FrameBufferPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	FrameBufferPipelineInfo.stageCount = 2;
	FrameBufferPipelineInfo.pStages = FrameBufferShaderStages;
	FrameBufferPipelineInfo.pVertexInputState = &emptyVertexInputStateCI;
	FrameBufferPipelineInfo.pInputAssemblyState = &inputAssembly;
	FrameBufferPipelineInfo.pViewportState = &viewportState;
	FrameBufferPipelineInfo.pRasterizationState = &rasterizer;
	FrameBufferPipelineInfo.pMultisampleState = &multisampling;
	FrameBufferPipelineInfo.pDepthStencilState = &depthStencil;
	FrameBufferPipelineInfo.pColorBlendState = &colorBlending;
	FrameBufferPipelineInfo.layout = ShaderPipelineLayout;
	FrameBufferPipelineInfo.renderPass = renderPass;
	FrameBufferPipelineInfo.subpass = 1;
	FrameBufferPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(DeviceInfo.Device, VK_NULL_HANDLE, 1, &FrameBufferPipelineInfo, nullptr, &ShaderPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(DeviceInfo.Device, FrameBufferFragShaderModule, nullptr);
	vkDestroyShaderModule(DeviceInfo.Device, FrameBufferVertShaderModule, nullptr);
}

void FrameBufferShader::CreateUniformBuffers()
{
}

void FrameBufferShader::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 4>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void FrameBufferShader::CreateDescriptorSets(VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView)
{
	BaseShader::CreateDescriptorSets();

	VkDescriptorImageInfo ColorImage = {};
	ColorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	ColorImage.imageView = PositionImageView;
	ColorImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo NormalImage = {};
	NormalImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	NormalImage.imageView = NormalImageView;
	NormalImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo AlbedoImage = {};
	AlbedoImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	AlbedoImage.imageView = AlbedoImageView;
	AlbedoImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo DepthImage = {};
	DepthImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DepthImage.imageView = DepthImageView;
	DepthImage.sampler = VK_NULL_HANDLE;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		std::array<WriteDescriptorSetInfo, 4>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[0].DescriptorImageInfo = ColorImage;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[1].DescriptorImageInfo = NormalImage;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[2].DescriptorImageInfo = AlbedoImage;

		WriteDescriptorInfo[3].DstBinding = 3;
		WriteDescriptorInfo[3].DstSet = descriptorSets[i];
		WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[3].DescriptorImageInfo = DepthImage;

		BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}