#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(std::string TexturePath, int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> CommandBuffer, std::vector<Vertex> VertexData, std::vector<uint16_t> IndexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue, VkDescriptorSetLayout DescriptorSetLayout)
{
	VBO = VertexBufferObject<Vertex>(SwapChainSize, device, physicalDevice, VertexData, CommandPool, GraphicsQueue);
	IBO = IndexBufferObject(SwapChainSize, device, physicalDevice, IndexData, CommandPool, GraphicsQueue);
	texture = Texture(TexturePath, device, physicalDevice, CommandBuffer, CommandPool, GraphicsQueue);

	UniformBufferobject = UniformBufferObject<UniformBufferObject2>(SwapChainSize, device, physicalDevice);
	LightBufferStuff = UniformBufferObject<LightingStruct>(SwapChainSize, device, physicalDevice);

	SetUpDescriptorPool(SwapChainSize, device);
	SetUpDescriptorSets(SwapChainSize, device, DescriptorSetLayout);
}

Mesh::~Mesh()
{
}

void Mesh::SetUpDescriptorPool(int SwapChainSize, VkDevice device)
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

void Mesh::SetUpDescriptorSets(int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout)
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

	for (size_t i = 0; i < SwapChainSize; i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = UniformBufferobject.GetShaderBuffer()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.GetTextureImageView();
		imageInfo.sampler = texture.GetTextureSampler();

		VkDescriptorBufferInfo LightbufferInfo = {};
		LightbufferInfo.buffer = LightBufferStuff.GetShaderBuffer()[i];
		LightbufferInfo.offset = 0;
		LightbufferInfo.range = sizeof(LightingStruct);

		std::array<VkWriteDescriptorSet, 3> descriptorWrites = {};

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

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = DescriptorSets[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pBufferInfo = &LightbufferInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Mesh::UpdateUniformBuffers(UniformBufferObject2 ubo, LightingStruct light, uint32_t ImageIndex)
{
	UniformBufferobject.UpdateShaderBuffer(ubo, ImageIndex);
	LightBufferStuff.UpdateShaderBuffer(light, ImageIndex);
}

void Mesh::UpdateTexture(Texture updateTexture, int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout)
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

	for (size_t i = 0; i < SwapChainSize; i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = UniformBufferobject.GetShaderBuffer()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = updateTexture.GetTextureImageView();
		imageInfo.sampler = updateTexture.GetTextureSampler();

		VkDescriptorBufferInfo LightbufferInfo = {};
		LightbufferInfo.buffer = LightBufferStuff.GetShaderBuffer()[i];
		LightbufferInfo.offset = 0;
		LightbufferInfo.range = sizeof(LightingStruct);

		std::array<VkWriteDescriptorSet, 3> descriptorWrites = {};

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

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = DescriptorSets[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pBufferInfo = &LightbufferInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		
	}
}

void Mesh::Draw(VkCommandBuffer CommandBuffer, VkPipeline Pipeline, VkPipelineLayout PipeLineLayout, uint32_t Indices, int frame)
{
	VkDeviceSize Offset[] = { 0 };
	VkBuffer VertexBuffer[] = { VBO.GetVertexBuffer() };

	vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipeLineLayout, 0, 1, &DescriptorSets[frame], 0, nullptr);
	vkCmdBindVertexBuffers(CommandBuffer, 0, 1, VertexBuffer, Offset);
	vkCmdDraw(CommandBuffer, 36, 1, 0, 0);
	//vkCmdBindIndexBuffer(CommandBuffer, IBO.GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
	//vkCmdDrawIndexed(CommandBuffer, Indices, 1, 0, 0, 0);
}

Mesh& Mesh::operator=(const Mesh& rhs)
{

	VBO = rhs.VBO;
	IBO = rhs.IBO;
	texture = rhs.texture;
	UniformBufferobject = rhs.UniformBufferobject;
	LightBufferStuff = rhs.LightBufferStuff;
	DescriptorPool = rhs.DescriptorPool;
	DescriptorSets = rhs.DescriptorSets;
	return *this;
}
