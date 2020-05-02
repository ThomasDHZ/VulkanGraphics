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
	VertexSize = vertices.size();
	IndiceSize = 0;

	SetUpDescriptorPool(Renderer);
	SetUpDescriptorSets(Renderer);
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetUpDescriptorPool(VulkanRenderer& Renderer)
{
		std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};
	
		DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	
		BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBox::SetUpDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

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

void SkyBox::UpdateUniformBuffer(VulkanRenderer& Renderer, SkyBoxUniformBufferObject ubo, uint32_t currentImage)
{
	BaseMesh::UpdateUniformBuffer(Renderer, uniformBuffersMemory[currentImage], static_cast<void*>(&ubo), sizeof(ubo));
}

void SkyBox::Draw(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, VkCommandBuffer commandBuffer, size_t currentImage)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);
}
