#include "SkyBox.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBox::SkyBox() : BaseMesh()
{
}

SkyBox::SkyBox(Renderer& renderer, CubeMapTexture texture) : BaseMesh(renderer)
{
	CubeMap = texture;
	VertexSize = SkyBoxVertices.size();
	IndiceSize = 0;

	SetUpVertexBuffer(renderer);
	SetUpUniformBuffers(renderer);
	SetUpDescriptorPool(renderer);
	SetUpDescriptorSets(renderer);
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetUpVertexBuffer(Renderer& renderer)
{
	VkDeviceSize bufferSize = sizeof(SkyBoxVertices[0]) * SkyBoxVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, SkyBoxVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), stagingBuffer, vertexBuffer, bufferSize, *GetSecondaryCommandPool(renderer), *GetGraphicsQueue(renderer));

	vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);
}

void SkyBox::SetUpDescriptorPool(Renderer& renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBox::SetUpUniformBuffers(Renderer& renderer)
{
	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(SkyBoxPositionMatrix));
}

void SkyBox::SetUpDescriptorSets(Renderer& renderer)
{
	//BaseMesh::CreateDescriptorSets(renderer, *GetSkyboxDescriptorSetLayout(renderer));

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = CubeMap.textureImageView;
	imageInfo.sampler = CubeMap.textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
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

		vkUpdateDescriptorSets(*GetDevice(renderer), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBox::Draw(Renderer& renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	//vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipeline(renderer));
	//vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	//vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	//vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
}

void SkyBox::UpdateUniformBuffer(Renderer& renderer, Camera& camera)
{
	SkyBoxPositionMatrix positionMatrix = {};
	positionMatrix.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	positionMatrix.projection = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	positionMatrix.projection[1][1] *= -1;

	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
}

void SkyBox::Destory(Renderer& renderer)
{
	PositionMatrixBuffer.Destroy(renderer);

	BaseMesh::Destory(renderer);
}
