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

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, FrameBufferMesh& mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
}

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, Mesh2& mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipeline);
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
}

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList)
{
    for (auto mesh : MeshList)
    {
        VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
        VkDeviceSize offsets[] = { 0 };

        //{
        //    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipeline);
        //    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        //    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, MeshviewPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        //    if (mesh.IndexSize == 0)
        //    {
        //        vkCmdDraw(commandBuffer, mesh.VertexSize, 1, 0, 0);
        //    }
        //    else
        //    {
        //        vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        //        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
        //    }
        //}
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipeline);
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, forwardRendereringPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
        }
    }
}

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, SkyBoxMesh mesh)
{
    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
    VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline.ShaderPipeline);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipeline.ShaderPipelineLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
	vkCmdDraw(commandBuffer, mesh.VertexSize, 1, 0, 0);
}

void RendererBase::Destroy(VkDevice Device)
{
    forwardRendereringPipeline.Destroy();
    skyboxPipeline.Destroy();
    frameBufferPipeline.Destroy();
    DebugLightPipeline.Destroy();
    DebugCollisionPipeline.Destroy();
    MeshviewPipeline.Destroy();

    vkDestroyRenderPass(Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
        vkDestroyFramebuffer(Device, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
}
