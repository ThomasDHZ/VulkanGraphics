#include "ShadowRenderer.h"
ShadowRenderer::ShadowRenderer() : RendererBase()
{
}

ShadowRenderer::ShadowRenderer(VulkanRenderer& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    CreateRendererFramebuffers(renderer);

    shadowPipeline = ShadowRenderingPipeline(renderer, RenderPass);
}

ShadowRenderer::~ShadowRenderer()
{
}

void ShadowRenderer::CreateRenderPass(VulkanRenderer& renderer)
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

void ShadowRenderer::CreateRendererFramebuffers(VulkanRenderer& renderer)
{
    SwapChainFramebuffers.resize(3);
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbufCreateInfo.renderPass = RenderPass;
        fbufCreateInfo.attachmentCount = 1;
        fbufCreateInfo.pAttachments = &DepthTexture.View;
        fbufCreateInfo.width = renderer.SwapChain.GetSwapChainResolution().width;
        fbufCreateInfo.height = renderer.SwapChain.GetSwapChainResolution().height;
        fbufCreateInfo.layers = 1;

        if (vkCreateFramebuffer(renderer.Device, &fbufCreateInfo, nullptr, &SwapChainFramebuffers[i]))
        {
            throw std::runtime_error("failed to create vkCreateImageView!");
        }
    }
}

void ShadowRenderer::UpdateSwapChain(VulkanRenderer& renderer)
{
    DepthTexture.RecreateRendererTexture(renderer);

    shadowPipeline.UpdateGraphicsPipeLine(renderer, RenderPass);

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    CreateRendererFramebuffers(renderer);
}

void ShadowRenderer::Destroy(VulkanRenderer& renderer)
{
    DepthTexture.Delete(renderer);
    RendererBase::Destroy(renderer);
}