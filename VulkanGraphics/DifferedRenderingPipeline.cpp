//#include "DifferedRenderingPipeline.h"
//#include <array>
//#include "Mesh.h"
//#include "Vertex.h"
//
//FramebufferPipeline::FramebufferPipeline() : GraphicsPipeline()
//{
//}
//
//FramebufferPipeline::FramebufferPipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass, VulkanDevice deviceInfo) : GraphicsPipeline(deviceInfo)
//{
//	DeviceInfo = deviceInfo;
//	CreateDescriptorSetLayout();
//	CreateShaderPipeLine(swapChainExtent, renderPass);
//}
//
//FramebufferPipeline::~FramebufferPipeline()
//{
//}
//
//void FramebufferPipeline::CreateDescriptorSetLayout()
//{
//	std::array<DescriptorSetLayoutBindingInfo, 6> LayoutBindingInfo = {};
//
//	LayoutBindingInfo[0].Binding = 0;
//	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
//	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	LayoutBindingInfo[1].Binding = 1;
//	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
//	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	LayoutBindingInfo[2].Binding = 2;
//	LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
//	LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	LayoutBindingInfo[3].Binding = 3;
//	LayoutBindingInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
//	LayoutBindingInfo[3].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	LayoutBindingInfo[4].Binding = 4;
//	LayoutBindingInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	LayoutBindingInfo[4].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	LayoutBindingInfo[5].Binding = 5;
//	LayoutBindingInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	LayoutBindingInfo[5].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//	GraphicsPipeline::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
//}
//
//void FramebufferPipeline::CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass)
//{
//	auto FrameBufferVertShaderCode = ReadShaderFile("shaders/FrameBufferVert.spv");
//	auto FrameBufferFrageShaderCode = ReadShaderFile("shaders/FrameBufferFrag.spv");
//
//	VkShaderModule FrameBufferVertShaderModule = CreateShaderModule(FrameBufferVertShaderCode);
//	VkShaderModule FrameBufferFragShaderModule = CreateShaderModule(FrameBufferFrageShaderCode);
//
//	VkPipelineShaderStageCreateInfo FrameBufferVertShaderStageInfo = {};
//	FrameBufferVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//	FrameBufferVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//	FrameBufferVertShaderStageInfo.module = FrameBufferVertShaderModule;
//	FrameBufferVertShaderStageInfo.pName = "main";
//
//	VkPipelineShaderStageCreateInfo FrameBufferFragShaderStageInfo = {};
//	FrameBufferFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//	FrameBufferFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//	FrameBufferFragShaderStageInfo.module = FrameBufferFragShaderModule;
//	FrameBufferFragShaderStageInfo.pName = "main";
//
//	VkPipelineShaderStageCreateInfo FrameBufferShaderStages[] = { FrameBufferVertShaderStageInfo, FrameBufferFragShaderStageInfo };
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
//	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
//	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//	colorBlendAttachment.blendEnable = VK_FALSE;
//
//	VkPipelineColorBlendStateCreateInfo colorBlending = {};
//	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//	colorBlending.logicOpEnable = VK_FALSE;
//	colorBlending.logicOp = VK_LOGIC_OP_COPY;
//	colorBlending.attachmentCount = 1;
//	colorBlending.pAttachments = &colorBlendAttachment;
//	colorBlending.blendConstants[0] = 0.0f;
//	colorBlending.blendConstants[1] = 0.0f;
//	colorBlending.blendConstants[2] = 0.0f;
//	colorBlending.blendConstants[3] = 0.0f;
//
//	VkPipelineVertexInputStateCreateInfo emptyVertexInputStateCI{};
//	emptyVertexInputStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//
//	VkPipelineLayoutCreateInfo FrameBufferPipelineLayoutInfo = {};
//	FrameBufferPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//	FrameBufferPipelineLayoutInfo.setLayoutCount = 1;
//	FrameBufferPipelineLayoutInfo.pSetLayouts = &ShaderPipelineDescriptorLayout;
//
//	GraphicsPipeline::CreatePipeLineLayout(FrameBufferPipelineLayoutInfo);
//
//	VkGraphicsPipelineCreateInfo FrameBufferPipelineInfo = {};
//	FrameBufferPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//	FrameBufferPipelineInfo.stageCount = 2;
//	FrameBufferPipelineInfo.pStages = FrameBufferShaderStages;
//	FrameBufferPipelineInfo.pVertexInputState = &emptyVertexInputStateCI;
//	FrameBufferPipelineInfo.pInputAssemblyState = &inputAssembly;
//	FrameBufferPipelineInfo.pViewportState = &viewportState;
//	FrameBufferPipelineInfo.pRasterizationState = &rasterizer;
//	FrameBufferPipelineInfo.pMultisampleState = &multisampling;
//	FrameBufferPipelineInfo.pDepthStencilState = &depthStencil;
//	FrameBufferPipelineInfo.pColorBlendState = &colorBlending;
//	FrameBufferPipelineInfo.layout = ShaderPipelineLayout;
//	FrameBufferPipelineInfo.renderPass = renderPass;
//	FrameBufferPipelineInfo.subpass = 1;
//	FrameBufferPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//
//	GraphicsPipeline::CreatePipeLine(FrameBufferPipelineInfo);
//
//	vkDestroyShaderModule(DeviceInfo.Device, FrameBufferFragShaderModule, nullptr);
//	vkDestroyShaderModule(DeviceInfo.Device, FrameBufferVertShaderModule, nullptr);
//}
//
//void FramebufferPipeline::RecreatePipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass)
//{
//	CreateDescriptorSetLayout();
//	CreateShaderPipeLine(swapChainExtent, renderPass);
//}