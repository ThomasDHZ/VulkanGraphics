#include "TextureRenderer.h"
#include <stdexcept>
#include <array>
#include "Vertex.h"

TextureRenderer::TextureRenderer() : RendererBase()
{
}

TextureRenderer::TextureRenderer(VulkanEngine& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    ColorTexture = std::make_shared<RendererColorTexture>(renderer);
    CreateRendererFramebuffers(renderer);
    SetUpColorBlendingSettings();

    forwardRendereringPipeline = std::make_shared<ForwardRenderingPipeline>(renderer, RenderPass);
    renderer2DPipeline = std::make_shared <Rendering2DPipeline>(Rendering2DPipeline(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_TextureRenderer));
    bloomPipeline = std::make_shared <BloomPipeline>(renderer, RenderPass);
    //reflection2DPipeline = Reflection2DPipeline(renderer, RenderPass);
    skyboxPipeline = std::make_shared<SkyBoxPipeline>(renderer, RenderPass);
    DebugLightPipeline = std::make_shared<DebugLightRenderingPipeline>(renderer, RenderPass);
    wireFramePipeline = std::make_shared<WireFramePipeline>(renderer, RenderPass);

    ImGui_ImplVulkan_AddTexture(ColorTexture->ImGuiDescriptorSet, ColorTexture->GetTextureSampler(), ColorTexture->GetTextureView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

TextureRenderer::~TextureRenderer()
{
}

void TextureRenderer::SetUpColorBlendingSettings()
{
    ColorBlendAttachment.resize(1);
    ColorBlendAttachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorBlendAttachment[0].blendEnable = VK_TRUE;
    ColorBlendAttachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorBlendAttachment[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorBlendAttachment[0].colorBlendOp = VK_BLEND_OP_ADD;
    ColorBlendAttachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorBlendAttachment[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorBlendAttachment[0].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    VkPipelineColorBlendStateCreateInfo ColorBlending = {};
    ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    ColorBlending.logicOpEnable = VK_FALSE;
    ColorBlending.logicOp = VK_LOGIC_OP_COPY;
    ColorBlending.attachmentCount = static_cast<uint32_t>(ColorBlendAttachment.size());
    ColorBlending.pAttachments = ColorBlendAttachment.data();
    ColorBlending.blendConstants[0] = 0.0f;
    ColorBlending.blendConstants[1] = 0.0f;
    ColorBlending.blendConstants[2] = 0.0f;
    ColorBlending.blendConstants[3] = 0.0f;
    ColorBlendingSettings = ColorBlending;
}

void TextureRenderer::CreateRenderPass(VulkanEngine& renderer)
{
    std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};

    attchmentDescriptions[0].format = VK_FORMAT_R8G8B8A8_UNORM;
    attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    attchmentDescriptions[1].format = VK_FORMAT_D32_SFLOAT;
    attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;

    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
    renderPassInfo.pAttachments = attchmentDescriptions.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(renderer.Device, &renderPassInfo, nullptr, &RenderPass))
    {
        throw std::runtime_error("failed to create vkCreateImageView!");
    }
}

void TextureRenderer::CreateRendererFramebuffers(VulkanEngine& renderer)
{
    SwapChainFramebuffers.resize(3);
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkImageView attachments[2];
        attachments[0] = ColorTexture->View;
        attachments[1] = DepthTexture.View;

        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbufCreateInfo.renderPass = RenderPass;
        fbufCreateInfo.attachmentCount = 2;
        fbufCreateInfo.pAttachments = attachments;
        fbufCreateInfo.width = renderer.SwapChain.GetSwapChainResolution().width;
        fbufCreateInfo.height = renderer.SwapChain.GetSwapChainResolution().height;
        fbufCreateInfo.layers = 1;

        if (vkCreateFramebuffer(renderer.Device, &fbufCreateInfo, nullptr, &SwapChainFramebuffers[i]))
        {
            throw std::runtime_error("failed to create vkCreateImageView!");
        }
    }
}

void TextureRenderer::UpdateSwapChain(VulkanEngine& renderer)
{
    ColorTexture->RecreateRendererTexture(renderer);
    DepthTexture.RecreateRendererTexture(renderer);

    forwardRendereringPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
    renderer2DPipeline->UpdateGraphicsPipeLine(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_TextureRenderer);
    bloomPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   // reflection2DPipeline.UpdateGraphicsPipeLine(renderer, RenderPass);
    skyboxPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
    DebugLightPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
    wireFramePipeline->UpdateGraphicsPipeLine(renderer, RenderPass);

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    CreateRendererFramebuffers(renderer);
}

void TextureRenderer::Destroy(VulkanEngine& renderer)
{
    ColorTexture->Delete(renderer);
    DepthTexture.Delete(renderer);

    forwardRendereringPipeline->Destroy(renderer);
    renderer2DPipeline->Destroy(renderer);
   // reflection2DPipeline.Destroy(renderer);
    bloomPipeline->Destroy(renderer);
    skyboxPipeline->Destroy(renderer);
    DebugLightPipeline->Destroy(renderer);
    wireFramePipeline->Destroy(renderer);

    RendererBase::Destroy(renderer);
}
