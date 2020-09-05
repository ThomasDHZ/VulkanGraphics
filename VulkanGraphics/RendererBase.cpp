#include "RendererBase.h"
#include "ImGui/imgui_impl_vulkan.h"

RendererBase::RendererBase()
{
}

RendererBase::RendererBase(VulkanRenderer& renderer)
{
}

RendererBase::~RendererBase()
{
}

void RendererBase::Draw(VulkanRenderer& renderer, GraphicsPipeline pipeline, const std::shared_ptr<BaseMesh> mesh)
{

    VkBuffer vertexBuffers[] = { mesh->VertexBuffer };
    VkDeviceSize offsets[] = { 0 };

    {
        vkCmdBindPipeline(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[renderer.DrawFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[renderer.DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipelineLayout, 0, 1, &mesh->DescriptorSets[renderer.DrawFrame], 0, nullptr);
        if (mesh->IndexSize == 0)
        {
            vkCmdDraw(renderer.RenderCommandBuffer[renderer.DrawFrame], mesh->VertexSize, 1, 0, 0);
        }
        else
        {
            vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[renderer.DrawFrame], mesh->IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(renderer.RenderCommandBuffer[renderer.DrawFrame], static_cast<uint32_t>(mesh->IndexSize), 1, 0, 0, 0);
        }
    }
}

void RendererBase::Destroy(VulkanRenderer& renderer)
{
    vkDestroyRenderPass(renderer.Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}
