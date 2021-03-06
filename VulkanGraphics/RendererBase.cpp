#include "RendererBase.h"
#include "ImGui/imgui_impl_vulkan.h"

RendererBase::RendererBase()
{
}

RendererBase::RendererBase(VulkanEngine& renderer)
{
    ColorBlendAttachment = std::make_shared<std::vector<VkPipelineColorBlendAttachmentState>>();
}

RendererBase::~RendererBase()
{
}

void RendererBase::SetUpColorBlendingSettings()
{
}

void RendererBase::Draw(VulkanEngine& renderer, std::shared_ptr<RendererDrawMessage>& drawMessage) const
{
    VkBuffer vertexBuffers[] = { drawMessage->MeshVertex.GetVertexBuffer() };
    VkDeviceSize offsets[] = { 0 };

    {
        vkCmdBindPipeline(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, drawMessage->pipeline->ShaderPipeline);
        vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[renderer.DrawFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, drawMessage->pipeline->ShaderPipelineLayout, 0, 1, &drawMessage->DescriptorSets[renderer.DrawFrame], 0, nullptr);
        if (drawMessage->MeshIndices.GetIndiceBuffer() == 0)
        {
            vkCmdDraw(renderer.RenderCommandBuffer[renderer.DrawFrame], drawMessage->MeshVertex.GetVertexCount(), 1, 0, 0);
        }
        else
        {
            vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[renderer.DrawFrame], drawMessage->MeshIndices.GetIndiceBuffer(), 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(renderer.RenderCommandBuffer[renderer.DrawFrame], static_cast<uint32_t>(drawMessage->MeshIndices.GetIndiceCount()), 1, 0, 0, 0);
        }
    }
}

void RendererBase::Destroy(VulkanEngine& renderer)
{
    vkDestroyRenderPass(renderer.Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}
