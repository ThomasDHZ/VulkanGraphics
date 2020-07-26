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

void RendererBase::Draw(VulkanRenderer& renderer, int frame, FrameBufferMesh& mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    {
        vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[frame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(renderer.RenderCommandBuffer[frame], static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
}

void RendererBase::Draw(VulkanRenderer& renderer, int frame, Mesh2& mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };

    {
        vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        if (mesh.IndexSize == 0)
        {
            vkCmdDraw(renderer.RenderCommandBuffer[frame], mesh.VertexSize, 1, 0, 0);
        }
        else
        {
            vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[frame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(renderer.RenderCommandBuffer[frame], static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
        }
    }
    {
        vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[frame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(renderer.RenderCommandBuffer[frame], static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
}

void RendererBase::Draw(VulkanRenderer& renderer, int frame, std::vector<Mesh2>& MeshList)
{
    for (auto mesh : MeshList)
    {
        VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
        VkDeviceSize offsets[] = { 0 };

        {
            vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipeline);
            vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
            vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
            if (mesh.IndexSize == 0)
            {
                vkCmdDraw(renderer.RenderCommandBuffer[frame], mesh.VertexSize, 1, 0, 0);
            }
            else
            {
                vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[frame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
                vkCmdDrawIndexed(renderer.RenderCommandBuffer[frame], static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
            }
        }
        {

            vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipeline);
            vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(renderer.RenderCommandBuffer[frame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
            vkCmdDrawIndexed(renderer.RenderCommandBuffer[frame], static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
        }
    }
}

void RendererBase::Draw(VulkanRenderer& renderer, int frame, SkyBoxMesh mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline.ShaderPipeline);
	vkCmdBindVertexBuffers(renderer.RenderCommandBuffer[frame], 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(renderer.RenderCommandBuffer[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
	vkCmdDraw(renderer.RenderCommandBuffer[frame], mesh.VertexSize, 1, 0, 0);
}

void RendererBase::Destroy(VulkanRenderer& renderer)
{
    forwardRendereringPipeline.Destroy(renderer);
    skyboxPipeline.Destroy(renderer);
    frameBufferPipeline.Destroy(renderer);
    DebugLightPipeline.Destroy(renderer);
    DebugCollisionPipeline.Destroy(renderer);
    MeshviewPipeline.Destroy(renderer);

    vkDestroyRenderPass(renderer.Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}
