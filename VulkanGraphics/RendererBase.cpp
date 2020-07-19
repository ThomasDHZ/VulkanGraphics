#include "RendererBase.h"

RendererBase::RendererBase()
{
}

RendererBase::~RendererBase()
{
}

std::vector<char> RendererBase::ReadFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule RendererBase::CreateShaderModule(VkDevice Device, const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, Mesh2& mesh)
{
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[frame];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    {
        VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RendererPipeline);
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RendererLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
    vkCmdEndRenderPass(commandBuffer);
}

void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList)
{
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass;
    renderPassInfo.framebuffer = SwapChainFramebuffers[frame];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    for (auto mesh : MeshList)
    {
        VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RendererPipeline);
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RendererLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.IndexSize), 1, 0, 0, 0);
    }
    vkCmdEndRenderPass(commandBuffer);
}

//void RendererBase::Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, SkyBoxMesh mesh)
//{
//    VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
//	VkDeviceSize offsets[] = { 0 };
//
//	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, SkyboxPipeline.RendererPipeline);
//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
//	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, SkyboxPipeline.RendererLayout, 0, 1, &mesh.DescriptorSets[frame], 0, nullptr);
//	vkCmdDraw(commandBuffer, mesh.VertexSize, 1, 0, 0);
//}

void RendererBase::Destroy(VkDevice Device)
{
    vkDestroyPipeline(Device, RendererPipeline, nullptr);
    vkDestroyPipelineLayout(Device, RendererLayout, nullptr);
    vkDestroyDescriptorSetLayout(Device, DescriptorSetLayout, nullptr);
    RendererPipeline = VK_NULL_HANDLE;
    RendererLayout = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;

    vkDestroyRenderPass(Device, RenderPass, nullptr);
    RenderPass = VK_NULL_HANDLE;

    for (auto& framebuffer : SwapChainFramebuffers)
    {
	    vkDestroyFramebuffer(Device, framebuffer, nullptr);
	    framebuffer = VK_NULL_HANDLE;
    }
}
