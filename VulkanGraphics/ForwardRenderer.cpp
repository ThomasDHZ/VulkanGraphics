#include "ForwardRenderer.h"
#include <stdexcept>
#include <array>
#include "Vertex.h"
#include "VulkanBufferManager.h"

ForwardRenderer::ForwardRenderer() : RendererBase()
{
}

ForwardRenderer::ForwardRenderer(VulkanEngine& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    CreateRendererFramebuffers(renderer);
    SetUpColorBlendingSettings();

    frameBufferPipeline = std::make_shared<FrameBufferRenderingPipeline>(renderer, RenderPass);
    forwardRendereringPipeline = std::make_shared<ForwardRenderingPipeline>(renderer, RenderPass);
    reflection2DPipeline = std::make_shared<Reflection2DPipeline>(renderer, RenderPass);
    renderer2DPipeline = std::make_shared<Rendering2DPipeline>(Rendering2DPipeline(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_ForwardRenderer));
    underwater2DPipeline = std::make_shared<UnderWater2DPipeline>(renderer, RenderPass);
    skyboxPipeline = std::make_shared<SkyBoxPipeline>(renderer, RenderPass);
    DebugLightPipeline = std::make_shared<DebugLightRenderingPipeline>(renderer, RenderPass);
    collisionDebugPipeline = std::make_shared<CollisionDebugPipeline>(renderer, RenderPass);
    wireFramePipeline = std::make_shared<WireFramePipeline>(renderer, RenderPass);
}

ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::SetUpColorBlendingSettings()
{
    VkPipelineColorBlendAttachmentState ColorAttachment = {};
    ColorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment.blendEnable = VK_TRUE;
    ColorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
    ColorBlendAttachment->emplace_back(ColorAttachment);

    VkPipelineColorBlendStateCreateInfo ColorBlending = {};
    ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    ColorBlending.logicOpEnable = VK_FALSE;
    ColorBlending.logicOp = VK_LOGIC_OP_COPY;
    ColorBlending.attachmentCount = static_cast<uint32_t>(ColorBlendAttachment->size());
    ColorBlending.pAttachments = ColorBlendAttachment->data();
    ColorBlending.blendConstants[0] = 0.0f;
    ColorBlending.blendConstants[1] = 0.0f;
    ColorBlending.blendConstants[2] = 0.0f;
    ColorBlending.blendConstants[3] = 0.0f;
    ColorBlendingSettings = ColorBlending;
}

void ForwardRenderer::CreateRenderPass(VulkanEngine& renderer)
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
}


void ForwardRenderer::CreateRendererFramebuffers(VulkanEngine& renderer)
{
    SwapChainFramebuffers.resize(renderer.SwapChain.GetSwapChainImageViews().size());
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageViews().size(); i++) {
        std::array<VkImageView, 2> attachments = {
            renderer.SwapChain.GetSwapChainImageViews()[i],
            DepthTexture.GetTextureView()
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

void ForwardRenderer::UpdateSwapChain(VulkanEngine& renderer)
{
   DepthTexture.RecreateRendererTexture(renderer);

   forwardRendereringPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   renderer2DPipeline->UpdateGraphicsPipeLine(renderer, RenderPass, ColorBlendingSettings, RendererType::RT_ForwardRenderer);
   reflection2DPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   underwater2DPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   skyboxPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   DebugLightPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   collisionDebugPipeline->UpdateGraphicsPipeLine(renderer, RenderPass);
   wireFramePipeline->UpdateGraphicsPipeLine(renderer, RenderPass);

   for (auto& framebuffer : SwapChainFramebuffers)
   {
       vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
       framebuffer = VK_NULL_HANDLE;
   }
   CreateRendererFramebuffers(renderer);
}

void ForwardRenderer::Destroy(VulkanEngine& renderer)
{
    DepthTexture.Delete(renderer);

    forwardRendereringPipeline->Destroy(renderer);
    renderer2DPipeline->Destroy(renderer);
    reflection2DPipeline->Destroy(renderer);
    underwater2DPipeline->Destroy(renderer);
    skyboxPipeline->Destroy(renderer);
    DebugLightPipeline->Destroy(renderer);
    collisionDebugPipeline-> Destroy(renderer);
    wireFramePipeline->Destroy(renderer);

    RendererBase::Destroy(renderer);
}
