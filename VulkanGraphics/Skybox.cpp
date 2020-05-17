#include "SkyBox.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBox::SkyBox() : BaseMesh()
{
}

SkyBox::SkyBox(VulkanRenderer& Renderer, CubeMapTexture texture) : BaseMesh(Renderer)
{
	CubeMap = texture;
	VertexSize = SkyBoxVertices.size();
	IndiceSize = 0;

	SetUpVertexBuffer(Renderer);
	SetUpUniformBuffers(Renderer);
	SetUpDescriptorPool(Renderer);
	SetUpDescriptorSets(Renderer);
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetUpVertexBuffer(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(SkyBoxVertices[0]) * SkyBoxVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, SkyBoxVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), stagingBuffer, vertexBuffer, bufferSize, *GetSecondaryCommandPool(Renderer), *GetGraphicsQueue(Renderer));

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);
}

void SkyBox::SetUpDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBox::SetUpUniformBuffers(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer = UniformBuffer(Renderer, sizeof(SkyBoxPositionMatrix));
}

void SkyBox::SetUpDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetSkyboxDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = CubeMap.textureImageView;
	imageInfo.sampler = CubeMap.textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxPositionMatrix);

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(*GetDevice(Renderer), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBox::Draw(VulkanRenderer& Renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipeline(Renderer));
	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(Renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipelineLayout(Renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	vkCmdDraw(*GetSecondaryCommandBuffer(Renderer, currentFrame), VertexSize, 1, 0, 0);
}

void SkyBox::UpdateUniformBuffer(VulkanRenderer& Renderer, SkyBoxPositionMatrix positionMatrix)
{
	PositionMatrixBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&positionMatrix), Renderer.DrawFrame);
}

void SkyBox::Destory(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer.Destroy(Renderer);

	BaseMesh::Destory(Renderer);
}
