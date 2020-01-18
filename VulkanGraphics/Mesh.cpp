#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<Vertex> VertexData, std::vector<uint16_t> IndexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
{
	VBO = VertexBufferObject<Vertex>(SwapChainSize, device, physicalDevice, VertexData, CommandPool, GraphicsQueue);
	IBO = IndexBufferObject(SwapChainSize, device, physicalDevice, IndexData, CommandPool, GraphicsQueue);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(VkCommandBuffer CommandBuffer, VkPipeline Pipeline, VkPipelineLayout PipeLineLayout, VkDescriptorSet DescriptorSet, uint32_t Indices)
{
	VkDeviceSize Offset[] = { 0 };
	VkBuffer VertexBuffer[] = { VBO.GetVertexBuffer() };

	vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipeLineLayout, 0, 1, &DescriptorSet, 0, nullptr);
	vkCmdBindVertexBuffers(CommandBuffer, 0, 1, VertexBuffer, Offset);
	vkCmdBindIndexBuffer(CommandBuffer, IBO.GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(CommandBuffer, Indices, 1, 0, 0, 0);
}

Mesh& Mesh::operator=(const Mesh& rhs)
{
	VBO = rhs.VBO;
	IBO = rhs.IBO;
	return *this;
}
