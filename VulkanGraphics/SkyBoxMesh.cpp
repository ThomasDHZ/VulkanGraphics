#include "SkyBoxMesh.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBoxMesh::SkyBoxMesh() : BaseMesh()
{
}

SkyBoxMesh::SkyBoxMesh(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout layout, int skyboxtexture) : BaseMesh(RendererBitFlag::RenderOnTexturePass | RendererBitFlag::RenderOnMainPass)
{
	VertexSize = SkyBoxVertices.size();
	IndexSize = 0;

	SetUpVertexBuffer(renderer);
	SetUpUniformBuffers(renderer);
	SetUpDescriptorPool(renderer);
	SetUpDescriptorSets(renderer, textureManager, skyboxtexture, layout);
}

SkyBoxMesh::~SkyBoxMesh()
{
}

void SkyBoxMesh::SetUpVertexBuffer(VulkanRenderer& renderer)
{
	VkDeviceSize bufferSize = sizeof(SkyBoxVertices[0]) * SkyBoxVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(renderer.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, SkyBoxVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(renderer.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

	VulkanBufferManager::CopyBuffer(renderer, stagingBuffer, VertexBuffer, bufferSize);

	vkDestroyBuffer(renderer.Device, stagingBuffer, nullptr);
	vkFreeMemory(renderer.Device, stagingBufferMemory, nullptr);
}

void SkyBoxMesh::SetUpDescriptorPool(VulkanRenderer& renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBoxMesh::SetUpUniformBuffers(VulkanRenderer& renderer)
{
	PositionMatrixBuffer = VulkanUniformBuffer(renderer, sizeof(SkyBoxPositionMatrix));
}

void SkyBoxMesh::SetUpDescriptorSets(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, int skyboxtexture, VkDescriptorSetLayout layout)
{
	BaseMesh::CreateDescriptorSets(renderer, layout);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = textureManager->GetTexture(skyboxtexture).View;
	imageInfo.sampler = textureManager->GetTexture(skyboxtexture).Sampler;

	for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxPositionMatrix);

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

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

		vkUpdateDescriptorSets(renderer.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBoxMesh::UpdateUniformBuffer(VulkanRenderer& renderer, Camera& camera)
{
	SkyBoxPositionMatrix positionMatrix = {};
	positionMatrix.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	positionMatrix.projection = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
	positionMatrix.projection[1][1] *= -1;

	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
}

void SkyBoxMesh::Destory(VulkanRenderer& renderer)
{
	PositionMatrixBuffer.Destroy(renderer);
	BaseMesh::Destory(renderer);
}
