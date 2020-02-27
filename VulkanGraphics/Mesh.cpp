#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, ShaderTextureInputs shaderInput, std::vector<Vertex> vertices, std::vector<uint16_t> indices)
{
	DeviceInfo = deviceInfo;

	VerticeSize = vertices.size();
	IndiceSize = indices.size();
	shader = Shader(DeviceInfo, swapChainExtent, renderPass, shaderInput);

	CreateVertexBuffer(vertices);
	CreateIndiceBuffer(indices);
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, vertexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void Mesh::CreateIndiceBuffer(std::vector<uint16_t> indices)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, indexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void Mesh::UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage)
{
	shader.UpdateUniformBuffer(shader.uniformBuffersMemory[currentImage], static_cast<void*>(&ubo2), sizeof(ubo2));
}

void Mesh::Draw(VkCommandBuffer commandbuffer, int currentImage)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.ShaderPipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.ShaderPipelineLayout, 0, 1, &shader.descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandbuffer, VerticeSize, 1, 0, 0);
	//vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
	//vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Mesh::RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, ShaderTextureInputs shaderInput)
{
	shader.RecreateSwapChainInfo(swapChainExtent, renderPass, shaderInput);
}

void Mesh::Destroy()
{
	vkDestroyBuffer(DeviceInfo.Device, indexBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, indexBufferMemory, nullptr);

	vkDestroyBuffer(DeviceInfo.Device, vertexBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, vertexBufferMemory, nullptr);

	shader.Destory();
}

void Mesh::DestroySwapChainStage()
{
	shader.DestorySwapChain();
}

Mesh& Mesh::operator=(const Mesh& rhs)
{
	DeviceInfo = rhs.DeviceInfo;

	VerticeSize = rhs.VerticeSize;
	IndiceSize = rhs.IndiceSize;
	shader = rhs.shader;

	vertexBuffer = rhs.vertexBuffer;
	vertexBufferMemory = rhs.vertexBufferMemory;

	indexBuffer = rhs.indexBuffer;
	indexBufferMemory = rhs.indexBufferMemory;
	return *this;
}
