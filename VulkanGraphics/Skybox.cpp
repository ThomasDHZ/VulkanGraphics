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

	VulkanBufferManager::CopyBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), stagingBuffer, vertexBuffer, bufferSize, Renderer.SubCommandPool, *GetGraphicsQueue(Renderer));

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
	VkDeviceSize bufferSize = sizeof(SkyBoxUniformBufferObject);

	uniformBuffers.resize(GetSwapChainImageCount(Renderer));
	uniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void SkyBox::SetUpDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetSkyboxDescriptorSetLayout(Renderer));

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = CubeMap.textureImageView;
		imageInfo.sampler = CubeMap.textureSampler;

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

	vkCmdBindPipeline(Renderer.SubCommandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipeline(Renderer));
	vkCmdBindVertexBuffers(Renderer.SubCommandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(Renderer.SubCommandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipelineLayout(Renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	vkCmdDraw(Renderer.SubCommandBuffers[currentFrame], VertexSize, 1, 0, 0);
}

void SkyBox::UpdateUniformBuffer(VulkanRenderer& Renderer, SkyBoxUniformBufferObject ubo, uint32_t currentImage)
{
	BaseMesh::UpdateUniformBuffer(Renderer, uniformBuffersMemory[currentImage], static_cast<void*>(&ubo), sizeof(ubo));
}

void SkyBox::Destory(VulkanRenderer& Renderer)
{
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		if (uniformBuffers[i] != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(*GetDevice(Renderer), uniformBuffers[i], nullptr);
			vkFreeMemory(*GetDevice(Renderer), uniformBuffersMemory[i], nullptr);

			uniformBuffers[i] = VK_NULL_HANDLE;
			uniformBuffersMemory[i] = VK_NULL_HANDLE;
		}
	}

	BaseMesh::Destory(Renderer);
}
