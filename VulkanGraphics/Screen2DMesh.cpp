#include "Screen2DMesh.h"
#include "VulkanBufferManager.h"

Screen2DMesh::Screen2DMesh() : BaseMesh()
{
}

Screen2DMesh::Screen2DMesh(VulkanRenderer& Renderer, std::vector<Texture2D>& TextureList) : BaseMesh(Renderer, TextureList)
{
	VertexSize = ScreenVertices.size();
	IndiceSize = 0;

	CreateVertexBuffer(Renderer);
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer, TextureList[0], TextureList[1]);
}

Screen2DMesh::~Screen2DMesh()
{
}

void Screen2DMesh::CreateVertexBuffer(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(ScreenVertices[0]) * ScreenVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, ScreenVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), stagingBuffer, vertexBuffer, bufferSize, Renderer.MainCommandPool, *GetGraphicsQueue(Renderer));

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);
}

void Screen2DMesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Screen2DMesh::CreateDescriptorSets(VulkanRenderer& Renderer, const Texture2D& CurrentScreenTexture, const Texture2D& spriteLayer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo BackGroundLayer = {};
	BackGroundLayer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	BackGroundLayer.imageView = CurrentScreenTexture.textureImageView;
	BackGroundLayer.sampler = CurrentScreenTexture.textureSampler;

	VkDescriptorImageInfo SpriteLayer = {};
	SpriteLayer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpriteLayer.imageView = spriteLayer.textureImageView;
	SpriteLayer.sampler = spriteLayer.textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		std::array<WriteDescriptorSetInfo, 2>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[0].DescriptorImageInfo = BackGroundLayer;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = SpriteLayer;

		BaseMesh::CreateDescriptorSetsData(Renderer, std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Screen2DMesh::Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage)
{

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandbuffer, VertexSize, 1, 0, 0);
}

void Screen2DMesh::UpdateSwapChain(VulkanRenderer& Renderer, const Texture2D& CurrentScreenTexture, const Texture2D& SpriteScreenTexture)
{
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer, CurrentScreenTexture, SpriteScreenTexture);
}