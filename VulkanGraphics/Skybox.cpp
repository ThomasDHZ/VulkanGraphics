#include "SkyBox.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, int SwapChainSize, VkImageView textureImageView, VkSampler textureSampler)
{
	SetUpDescriptorSetLayout(device);
	SetUpVertexBuffer(SwapChainSize, device, physicalDevice, vertices, commandPool, queue);
	SetUpUniformBuffers(SwapChainSize, device, physicalDevice);
	SetUpDescriptorPool(device, SwapChainSize);
	SetUpDescriptorSets(device, SwapChainSize,  textureImageView,  textureSampler);
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetUpDescriptorSetLayout(VkDevice Device)
{
	VkDescriptorSetLayoutBinding UBOLayoutBinding = {};
	UBOLayoutBinding.binding = 0;
	UBOLayoutBinding.descriptorCount = 1;
	UBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	UBOLayoutBinding.pImmutableSamplers = nullptr;
	UBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding SamplerLayoutBinding = {};
	SamplerLayoutBinding.binding = 1;
	SamplerLayoutBinding.descriptorCount = 1;
	SamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	SamplerLayoutBinding.pImmutableSamplers = nullptr;
	SamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { UBOLayoutBinding, SamplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(Device, &layoutInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void SkyBox::SetUpVertexBuffer(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<SkyBoxVertex> VertexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
{
	VBO = VertexBufferObject<SkyBoxVertex>(SwapChainSize, device, physicalDevice, VertexData, CommandPool, GraphicsQueue);
}

void SkyBox::SetUpUniformBuffers(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice)
{
	UBO = UniformBufferObject<SkyBoxUniformBufferObject>(SwapChainSize, device, physicalDevice);
}

void SkyBox::SetUpDescriptorPool(VkDevice device, int SwapChainSize)
{
	std::array<VkDescriptorPoolSize, 2> PoolSizes = {};
	PoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	PoolSizes[0].descriptorCount = static_cast<uint32_t>(SwapChainSize);
	PoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	PoolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChainSize);

	VkDescriptorPoolCreateInfo PoolInfo = {};
	PoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	PoolInfo.poolSizeCount = static_cast<uint32_t>(PoolSizes.size());
	PoolInfo.pPoolSizes = PoolSizes.data();
	PoolInfo.maxSets = static_cast<uint32_t>(SwapChainSize);

	if (vkCreateDescriptorPool(device, &PoolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void SkyBox::SetUpDescriptorSets(VkDevice device, int SwapChainSize, VkImageView textureImageView, VkSampler textureSampler)
{
	std::vector<VkDescriptorSetLayout> layouts(SwapChainSize, DescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(SwapChainSize);
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < SwapChainSize; i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = UBO.GetShaderBuffer(i);
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

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

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBox::UpdateUniformBuffer(SkyBoxUniformBufferObject ubo, uint32_t currentImage)
{
	UBO.UpdateShaderBuffer(ubo, currentImage);
}

void SkyBox::Draw(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, VkCommandBuffer commandBuffer, size_t currentImage)
{
	VkBuffer vertexBuffers[] = { VBO.GetVertexBuffer() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);
}

void SkyBox::DestorySwapChain(VkDevice device, int SwapChainSize)
{
	UBO.Destroy(SwapChainSize);

	if (descriptorPool != nullptr)
	{
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		descriptorPool = nullptr;
	}
}

void SkyBox::Destory(VkDevice device, int SwapChainSize)
{
	DestorySwapChain(device, SwapChainSize);

	vkDestroyDescriptorSetLayout(device, DescriptorSetLayout, nullptr);
	vkDestroyBuffer(device, VBO.GetVertexBuffer(), nullptr);
	vkFreeMemory(device, VBO.GetVertexMemoryBuffer(), nullptr);
}

