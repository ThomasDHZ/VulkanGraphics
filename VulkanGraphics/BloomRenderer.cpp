#include "BloomRenderer.h"

#include <stdexcept>
#include <array>
#include "Vertex.h"

BloomRenderer::BloomRenderer() : RendererBase()
{
}

BloomRenderer::BloomRenderer(VulkanRenderer& renderer) : RendererBase(renderer)
{
    CreateRenderPass(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    ColorTexture = std::make_shared<RendererColorTexture>(renderer);
    BloomTexture = std::make_shared<RendererColorTexture>(renderer);
    CreateRendererFramebuffers(renderer);

    renderer2DPipeline2 = std::make_shared<Rendering2DPipeline2>(renderer, RenderPass);

    ImGui_ImplVulkan_AddTexture(ColorTexture->ImGuiDescriptorSet, ColorTexture->GetTextureSampler(), ColorTexture->GetTextureView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

BloomRenderer::BloomRenderer(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture>& texture)
{
    CreateRenderPass(renderer);
    DepthTexture = RendererDepthTexture(renderer);
    ColorTexture = std::make_shared<RendererColorTexture>(renderer);
    BloomTexture = std::make_shared<RendererColorTexture>(renderer);
    CreateRendererFramebuffers(renderer);

    renderer2DPipeline2 = std::make_shared<Rendering2DPipeline2>(renderer, RenderPass);

    ImGui_ImplVulkan_AddTexture(ColorTexture->ImGuiDescriptorSet, ColorTexture->GetTextureSampler(), ColorTexture->GetTextureView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

BloomRenderer::~BloomRenderer()
{
}

void BloomRenderer::CreateRenderPass(VulkanRenderer& renderer)
{
    std::array<VkAttachmentDescription, 3> attchmentDescriptions = {};

    attchmentDescriptions[0].format = VK_FORMAT_R8G8B8A8_UNORM;
    attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    attchmentDescriptions[1].format = VK_FORMAT_R8G8B8A8_UNORM;
    attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    attchmentDescriptions[2].format = VK_FORMAT_D32_SFLOAT;
    attchmentDescriptions[2].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[2].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[2].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    std::array<VkAttachmentReference, 2> ColorRefs;
    ColorRefs[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    ColorRefs[1] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthReference = { 2, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 2;
    subpassDescription.pColorAttachments = ColorRefs.data();
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

void BloomRenderer::CreateRendererFramebuffers(VulkanRenderer& renderer)
{
    SwapChainFramebuffers.resize(3);
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkImageView attachments[3];
        attachments[0] = ColorTexture->View;
        attachments[1] = BloomTexture->View;
        attachments[2] = DepthTexture.View;

        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbufCreateInfo.renderPass = RenderPass;
        fbufCreateInfo.attachmentCount = 3;
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

void BloomRenderer::CreateRendererFramebuffers(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture)
{
    SwapChainFramebuffers.resize(3);
    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkImageView attachments[3];
        attachments[0] = ColorTexture->View;
        attachments[1] = texture->View;
        attachments[2] = DepthTexture.View;

        VkFramebufferCreateInfo fbufCreateInfo = {};
        fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbufCreateInfo.renderPass = RenderPass;
        fbufCreateInfo.attachmentCount = 3;
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

void BloomRenderer::UpdateSwapChain(VulkanRenderer& renderer)
{
    ColorTexture->RecreateRendererTexture(renderer);
    BloomTexture->RecreateRendererTexture(renderer);
    DepthTexture.RecreateRendererTexture(renderer);

    renderer2DPipeline2->UpdateGraphicsPipeLine(renderer, RenderPass);

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    CreateRendererFramebuffers(renderer);
}

void BloomRenderer::UpdateSwapChain(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture)
{
    ColorTexture->RecreateRendererTexture(renderer);
    BloomTexture->RecreateRendererTexture(renderer);
    DepthTexture.RecreateRendererTexture(renderer);

    renderer2DPipeline2->UpdateGraphicsPipeLine(renderer, RenderPass);

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    CreateRendererFramebuffers(renderer, texture);
}

void BloomRenderer::Destroy(VulkanRenderer& renderer)
{
    ColorTexture->Delete(renderer);
    BloomTexture->Delete(renderer);
    DepthTexture.Delete(renderer);

    renderer2DPipeline2->Destroy(renderer);

    RendererBase::Destroy(renderer);
}