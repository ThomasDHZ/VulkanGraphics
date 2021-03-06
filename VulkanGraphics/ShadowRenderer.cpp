#include "ShadowRenderer.h"
ShadowRenderer::ShadowRenderer() : RendererBase()
{
}

ShadowRenderer::ShadowRenderer(VulkanEngine& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    DepthTexture = std::make_shared<RendererDepthTexture>(RendererDepthTexture(renderer));
    CreateRendererFramebuffers(renderer);

    forwardRendereringPipeline = std::make_shared<ShadowForwardRendereringPipeline>(renderer, RenderPass);
    renderer2DPipeline = std::make_shared<Rendering2DPipeline>(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_ShadowRenderer);
    ImGui_ImplVulkan_AddTexture(DepthTexture->ImGuiDescriptorSet, DepthTexture->GetTextureSampler(), DepthTexture->GetTextureView(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}

ShadowRenderer::~ShadowRenderer()
{
}

void ShadowRenderer::SetUpColorBlendingSettings()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

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
}

void ShadowRenderer::CreateRenderPass(VulkanEngine& renderer)
{
    VkAttachmentDescription attachmentDescription{};
    attachmentDescription.format = VK_FORMAT_D32_SFLOAT;
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;						
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;					
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 0;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;			

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;													
    subpass.pDepthStencilAttachment = &depthReference;									

    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachmentDescription;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(renderer.Device, &renderPassInfo, nullptr, &RenderPass))
    {
        throw std::runtime_error("failed to create vkCreateImageView!");
    }
}

void ShadowRenderer::CreateRendererFramebuffers(VulkanEngine& renderer)
{
    SwapChainFramebuffers.resize(3);
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbufCreateInfo.renderPass = RenderPass;
        fbufCreateInfo.attachmentCount = 1;
        fbufCreateInfo.pAttachments = &DepthTexture->View;
        fbufCreateInfo.width = renderer.SwapChain.GetSwapChainResolution().width;
        fbufCreateInfo.height = renderer.SwapChain.GetSwapChainResolution().height;
        fbufCreateInfo.layers = 1;

        if (vkCreateFramebuffer(renderer.Device, &fbufCreateInfo, nullptr, &SwapChainFramebuffers[i]))
        {
            throw std::runtime_error("failed to create vkCreateImageView!");
        }
    }
}

void ShadowRenderer::UpdateSwapChain(VulkanEngine& renderer)
{
    DepthTexture->RecreateRendererTexture(renderer);
    forwardRendereringPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
    renderer2DPipeline->UpdateGraphicsPipeLine(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_ShadowRenderer);
    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    CreateRendererFramebuffers(renderer);

    ImGui_ImplVulkan_AddTexture(DepthTexture->ImGuiDescriptorSet, DepthTexture->GetTextureSampler(), DepthTexture->GetTextureView(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}

void ShadowRenderer::Destroy(VulkanEngine& renderer)
{
    DepthTexture->Delete(renderer);
    forwardRendereringPipeline->Destroy(renderer);
    renderer2DPipeline->Destroy(renderer);
    RendererBase::Destroy(renderer);
}