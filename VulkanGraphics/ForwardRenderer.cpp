#include "ForwardRenderer.h"
#include <stdexcept>
#include <array>
#include "Vertex.h"
#include "VulkanBufferManager.h"

ForwardRenderer::ForwardRenderer() : RendererBase()
{
}

ForwardRenderer::ForwardRenderer(VulkanRenderer& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    CreateDescriptorSets(renderer);
    CreateRenderingPipeline(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    HDRColorTexture = RendererHDRColorTexture(renderer);
    CreateRendererFramebuffers(renderer);

    skyboxPipeline = SkyBoxPipeline(renderer.SwapChain.GetSwapChainResolution(), RenderPass, renderer.Device);
    DebugLightPipeline = DebugLightRenderingPipeline(renderer.SwapChain.GetSwapChainResolution(), RenderPass, renderer.Device);
    DebugCollisionPipeline = CollisionDebugPipeline(renderer.SwapChain.GetSwapChainResolution(), RenderPass, renderer.Device);
    MeshviewPipeline = WireFramePipeline(renderer.SwapChain.GetSwapChainResolution(), RenderPass, renderer.Device);
}

ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::CreateRenderPass(VulkanRenderer& renderer)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(renderer.Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

    //std::vector<VkAttachmentDescription> RenderPassAttachmentList;
    //std::vector<VkSubpassDescription> SubpassDescriptionList;
    //std::vector<VkSubpassDependency> SubpassDependencyList;

    //VkAttachmentDescription SwapChainAttachment = {};
    //SwapChainAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    //SwapChainAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    //SwapChainAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //SwapChainAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    //SwapChainAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //SwapChainAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //SwapChainAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //SwapChainAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //VkAttachmentDescription ColorAttachment = {};
    //ColorAttachment.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    //ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    //ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    //VkAttachmentDescription DepthAttachment = {};
    //DepthAttachment.format = VK_FORMAT_D32_SFLOAT;
    //DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    //DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    //DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //VkAttachmentReference colorReference = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    //VkAttachmentReference depthReference = { 2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    //VkAttachmentReference colorReferenceSwapchain = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    //VkAttachmentReference inputReferences[2];
    //inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
    //inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

    //VkSubpassDescription FirstSubPassDescription = {};
    //FirstSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //FirstSubPassDescription.colorAttachmentCount = 1;
    //FirstSubPassDescription.pColorAttachments = &colorReference;
    //FirstSubPassDescription.pDepthStencilAttachment = &depthReference;
    //FirstSubPassDescription.inputAttachmentCount = 0;

    //VkSubpassDescription SecondSubPassDescription = {};
    //SecondSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //SecondSubPassDescription.colorAttachmentCount = 1;
    //SecondSubPassDescription.pColorAttachments = &colorReferenceSwapchain;
    //SecondSubPassDescription.inputAttachmentCount = 2;
    //SecondSubPassDescription.pInputAttachments = inputReferences;


    //VkSubpassDependency FirstDependency = {};
    //FirstDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    //FirstDependency.dstSubpass = 0;
    //FirstDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    //FirstDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //FirstDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    //FirstDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //FirstDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    //VkSubpassDependency SecondDependency = {};
    //SecondDependency.srcSubpass = 0;
    //SecondDependency.dstSubpass = 1;
    //SecondDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //SecondDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    //SecondDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //SecondDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    //SecondDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    //VkSubpassDependency ThirdDependency = {};
    //ThirdDependency.srcSubpass = 0;
    //ThirdDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
    //ThirdDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //ThirdDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    //ThirdDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //ThirdDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    //ThirdDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    //RenderPassAttachmentList.emplace_back(SwapChainAttachment);
    //RenderPassAttachmentList.emplace_back(ColorAttachment);
    //RenderPassAttachmentList.emplace_back(DepthAttachment);

    //SubpassDescriptionList.emplace_back(FirstSubPassDescription);
    //SubpassDescriptionList.emplace_back(SecondSubPassDescription);

    //SubpassDependencyList.emplace_back(FirstDependency);
    //SubpassDependencyList.emplace_back(SecondDependency);
    //SubpassDependencyList.emplace_back(ThirdDependency);

    //VkRenderPassCreateInfo RenderPassInfo{};
    //RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    //RenderPassInfo.attachmentCount = static_cast<uint32_t>(RenderPassAttachmentList.size());
    //RenderPassInfo.pAttachments = RenderPassAttachmentList.data();
    //RenderPassInfo.subpassCount = static_cast<uint32_t>(SubpassDescriptionList.size());
    //RenderPassInfo.pSubpasses = SubpassDescriptionList.data();
    //RenderPassInfo.dependencyCount = static_cast<uint32_t>(SubpassDependencyList.size());
    //RenderPassInfo.pDependencies = SubpassDependencyList.data();

    //if (vkCreateRenderPass(renderer.Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
    //    throw std::runtime_error("failed to create render pass!");
    //}
}

void ForwardRenderer::CreateDescriptorSets(VulkanRenderer& renderer)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(renderer.Device, &layoutInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void ForwardRenderer::CreateRenderingPipeline(VulkanRenderer& renderer)
{
    auto vertShaderCode = ReadFile("shaders/ForwardRendererVert.spv");
    auto fragShaderCode = ReadFile("shaders/ForwardRendererFrag.spv");

    VkShaderModule vertShaderModule = CreateShaderModule(renderer.Device, vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(renderer.Device, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)renderer.SwapChain.GetSwapChainResolution().width;
    viewport.height = (float)renderer.SwapChain.GetSwapChainResolution().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = renderer.SwapChain.GetSwapChainResolution();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;

    if (vkCreatePipelineLayout(renderer.Device, &pipelineLayoutInfo, nullptr, &RendererLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = RendererLayout;
    pipelineInfo.renderPass = RenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(renderer.Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &RendererPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(renderer.Device, fragShaderModule, nullptr);
    vkDestroyShaderModule(renderer.Device, vertShaderModule, nullptr);
}

void ForwardRenderer::CreateRendererFramebuffers(VulkanRenderer& renderer)
{
    SwapChainFramebuffers.resize(renderer.SwapChain.GetSwapChainImageViews().size());
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageViews().size(); i++) {
        std::array<VkImageView, 2> attachments = {
            renderer.SwapChain.GetSwapChainImageViews()[i],
           // HDRColorTexture.View,
            DepthTexture.View
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = renderer.SwapChain.GetSwapChainResolution().width;
        framebufferInfo.height = renderer.SwapChain.GetSwapChainResolution().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(renderer.Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void ForwardRenderer::UpdateSwapChain(VulkanRenderer& renderer)
{
   HDRColorTexture = RendererHDRColorTexture(renderer);
   DepthTexture = RendererDepthTexture(renderer);
   CreateRenderingPipeline(renderer);
   CreateRendererFramebuffers(renderer);
}

void ForwardRenderer::Destroy(VulkanRenderer& renderer)
{
    HDRColorTexture.Delete(renderer);
    DepthTexture.Delete(renderer);
    RendererBase::Destroy(renderer.Device);
}
