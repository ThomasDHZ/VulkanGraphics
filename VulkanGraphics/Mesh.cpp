#include "Mesh.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, const std::vector<Texture2D>& textureList) : BaseMesh(deviceInfo, vertexList, indexList, textureList)
{
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
}

Mesh::~Mesh()
{
}

void Mesh::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Mesh::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 3>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets()
{
	BaseMesh::CreateDescriptorSets(DeviceInfo.descriptorSetLayout);

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList[0].textureImageView;
	DiffuseMap.sampler = TextureList[0].textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureList[1].textureImageView;
	SpecularMap.sampler = TextureList[1].textureSampler;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		std::array<WriteDescriptorSetInfo, 3>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		Mesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Mesh::Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage)
{

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandbuffer, VertexSize, 1, 0, 0);
	if (IndiceSize == 0)
	{
		vkCmdDraw(commandbuffer, VertexSize, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(commandbuffer, static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
	}
}

void Mesh::UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage)
{
	BaseMesh::UpdateUniformBuffer(uniformBuffersMemory[currentImage], static_cast<void*>(&ubo2), sizeof(ubo2));
}

void Mesh::UpdateSwapChain()
{
 	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
}

void Mesh::ClearSwapChain()
{
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++) 
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[i], nullptr);
		vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[i], nullptr);
	}

	BaseMesh::ClearSwapChain();
}

