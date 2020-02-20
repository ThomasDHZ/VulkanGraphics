#include "SkyBoxShader.h"

SkyBoxShader::SkyBoxShader() : BaseShader()
{
}

SkyBoxShader::SkyBoxShader(VulkanDevice deviceInfo) : BaseShader(deviceInfo)
{
	CreateDescriptorSetLayout();
}

SkyBoxShader::~SkyBoxShader()
{
}

void SkyBoxShader::CreateDescriptorSetLayout()
{
	std::array<DescriptorSetLayoutBindingInfo, 5> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	BaseShader::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void SkyBoxShader::CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass)
{
	auto SkyBoxvertShaderCode = ReadShaderFile("Shaders/SkyBoxVert.spv");
	auto SkyBoxfragShaderCode = ReadShaderFile("Shaders/SkyBoxFrag.spv");

	VkShaderModule SkyBoxvertShaderModule = CreateShaderModule(SkyBoxvertShaderCode);
	VkShaderModule SkyBoxfragShaderModule = CreateShaderModule(SkyBoxfragShaderCode);

	VkPipelineShaderStageCreateInfo SkyBoxvertShaderStageInfo = {};
	SkyBoxvertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxvertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	SkyBoxvertShaderStageInfo.module = SkyBoxvertShaderModule;
	SkyBoxvertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxfragShaderStageInfo = {};
	SkyBoxfragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	SkyBoxfragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	SkyBoxfragShaderStageInfo.module = SkyBoxfragShaderModule;
	SkyBoxfragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo SkyBoxshaderStages[] = { SkyBoxvertShaderStageInfo, SkyBoxfragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo SkyBoxvertexInputInfo = {};
	SkyBoxvertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto SkyBoxbindingDescription = SkyBoxVertex::getBindingDescription();
	auto SkyBoxattributeDescriptions = SkyBoxVertex::getAttributeDescriptions();

	SkyBoxvertexInputInfo.vertexBindingDescriptionCount = 1;
	SkyBoxvertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(SkyBoxattributeDescriptions.size());
	SkyBoxvertexInputInfo.pVertexBindingDescriptions = &SkyBoxbindingDescription;
	SkyBoxvertexInputInfo.pVertexAttributeDescriptions = SkyBoxattributeDescriptions.data();

	VkPipelineDepthStencilStateCreateInfo SkyBoxdepthStencil = {};
	SkyBoxdepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	SkyBoxdepthStencil.depthTestEnable = VK_TRUE;
	SkyBoxdepthStencil.depthWriteEnable = VK_TRUE;
	SkyBoxdepthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	SkyBoxdepthStencil.depthBoundsTestEnable = VK_FALSE;
	SkyBoxdepthStencil.stencilTestEnable = VK_FALSE;

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

	VkPipelineLayoutCreateInfo SkyBoxpipelineLayoutInfo = {};
	SkyBoxpipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	SkyBoxpipelineLayoutInfo.setLayoutCount = 1;
	SkyBoxpipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(DeviceInfo.Device, &SkyBoxpipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo SkyBoxpipelineInfo = {};
	SkyBoxpipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	SkyBoxpipelineInfo.stageCount = 2;
	SkyBoxpipelineInfo.pStages = SkyBoxshaderStages;
	SkyBoxpipelineInfo.pVertexInputState = &SkyBoxvertexInputInfo;
	SkyBoxpipelineInfo.pInputAssemblyState = &inputAssembly;
	SkyBoxpipelineInfo.pViewportState = &viewportState;
	SkyBoxpipelineInfo.pRasterizationState = &rasterizer;
	SkyBoxpipelineInfo.pMultisampleState = &multisampling;
	SkyBoxpipelineInfo.pDepthStencilState = &SkyBoxdepthStencil;
	SkyBoxpipelineInfo.pColorBlendState = &colorBlending;
	SkyBoxpipelineInfo.layout = ShaderPipelineLayout;
	SkyBoxpipelineInfo.renderPass = renderPass;
	SkyBoxpipelineInfo.subpass = 0;
	SkyBoxpipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(DeviceInfo.Device, VK_NULL_HANDLE, 1, &SkyBoxpipelineInfo, nullptr, &ShaderPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(DeviceInfo.Device, SkyBoxfragShaderModule, nullptr);
	vkDestroyShaderModule(DeviceInfo.Device, SkyBoxvertShaderModule, nullptr);
}

void SkyBoxShader::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(SkyBoxUniformBufferObject);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void SkyBoxShader::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBoxShader::CreateDescriptorSets(VkImageView textureImageView, VkSampler textureSampler)
{
	BaseShader::CreateDescriptorSets();

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}
