#include "CubeMapMesh.h"

CubeMapMesh::CubeMapMesh()
{

}

CubeMapMesh::CubeMapMesh(std::vector<std::string> TexturePath, int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> CommandBuffer, std::vector<CubeMapVertex> VertexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue, VkDescriptorSetLayout DescriptorSetLayout)
{
	VBO = VertexBufferObject<CubeMapVertex>(SwapChainSize, device, physicalDevice, VertexData, CommandPool, GraphicsQueue);
	texture = CubeTexture(TexturePath, device, physicalDevice, CommandBuffer, CommandPool, GraphicsQueue);

	UniformBufferobject = UniformBufferObject<SkyBoxUniformBufferObject2>(SwapChainSize, device, physicalDevice);

	SetUpDescriptorPool(SwapChainSize, device);
	SetUpDescriptorSets(SwapChainSize, device, DescriptorSetLayout);
}

CubeMapMesh::~CubeMapMesh()
{
}

void CubeMapMesh::SetUpDescriptorPool(int SwapChainSize, VkDevice device)
{
	std::array<VkDescriptorPoolSize, 2> PoolSizes = {};
	PoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	PoolSizes[0].descriptorCount = static_cast<unsigned int>(SwapChainSize);
	PoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	PoolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChainSize);

	VkDescriptorPoolCreateInfo DescriptorPoolInfo = {};
	DescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(PoolSizes.size());
	DescriptorPoolInfo.pPoolSizes = PoolSizes.data();
	DescriptorPoolInfo.maxSets = static_cast<unsigned int>(SwapChainSize);

	VkResult Result = vkCreateDescriptorPool(device, &DescriptorPoolInfo, nullptr, &DescriptorPool);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool.");
	}
}

void CubeMapMesh::SetUpDescriptorSets(int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout)
{
	std::vector<VkDescriptorSetLayout> layouts(SwapChainSize, DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	DescriptorSets.resize(SwapChainSize);
	if (vkAllocateDescriptorSets(device, &allocInfo, DescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < SwapChainSize; i++) 
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = UniformBufferobject.GetShaderBuffer()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject2);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.GetTextureImageView();
		imageInfo.sampler = texture.GetTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void CubeMapMesh::UpdateUniformBuffers(SkyBoxUniformBufferObject2 ubo, uint32_t ImageIndex)
{
	UniformBufferobject.UpdateShaderBuffer(ubo, ImageIndex);
}

void CubeMapMesh::UpdateTexture(CubeTexture updateTexture, int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout)
{
	DescriptorSets.clear();

	std::array<VkDescriptorPoolSize, 2> PoolSizes = {};
	PoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	PoolSizes[0].descriptorCount = static_cast<unsigned int>(SwapChainSize);
	PoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	PoolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChainSize);

	VkDescriptorPoolCreateInfo DescriptorPoolInfo = {};
	DescriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(PoolSizes.size());
	DescriptorPoolInfo.pPoolSizes = PoolSizes.data();
	DescriptorPoolInfo.maxSets = static_cast<unsigned int>(SwapChainSize);

	VkResult Result = vkCreateDescriptorPool(device, &DescriptorPoolInfo, nullptr, &DescriptorPool);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool.");
	}

	std::vector<VkDescriptorSetLayout> layouts(SwapChainSize, DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	DescriptorSets.resize(SwapChainSize);

	Result = vkAllocateDescriptorSets(device, &allocInfo, DescriptorSets.data());
	if (Result != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < SwapChainSize; i++) 
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = UniformBufferobject.GetShaderBuffer()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject2);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = updateTexture.GetTextureImageView();
		imageInfo.sampler = updateTexture.GetTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void CubeMapMesh::Draw(VkCommandBuffer CommandBuffer, VkPipeline Pipeline, VkPipelineLayout PipeLineLayout, int frame)
{
	VkDeviceSize Offset[] = { 0 };
	VkBuffer VertexBuffer[] = { VBO.GetVertexBuffer() };

	vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipeLineLayout, 0, 1, &DescriptorSets[frame], 0, nullptr);
	vkCmdBindVertexBuffers(CommandBuffer, 0, 1, VertexBuffer, Offset);
	vkCmdDraw(CommandBuffer, 36, 1, 0, 0);
}

void CubeMapMesh::Destory(VkDevice device, int FrameSize)
{
	UniformBufferobject.Destroy(FrameSize);
	texture.Destory();
	vkDestroyDescriptorPool(device, DescriptorPool, nullptr);
}

CubeMapMesh& CubeMapMesh::operator=(const CubeMapMesh& rhs)
{

	VBO = rhs.VBO;
	texture = rhs.texture;
	UniformBufferobject = rhs.UniformBufferobject;
	DescriptorPool = rhs.DescriptorPool;
	DescriptorSets = rhs.DescriptorSets;
	return *this;
}
