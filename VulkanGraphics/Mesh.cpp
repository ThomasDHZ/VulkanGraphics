#include "Mesh.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, const std::vector<Texture2D>& textureList) : BaseMesh(Renderer, vertexList, indexList, textureList)
{
	CreateUniformBuffers(Renderer);
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer);
}

Mesh::~Mesh()
{
}

void Mesh::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(GetSwapChainImageCount(Renderer));
	uniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Mesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 3>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList[0].textureImageView;
	DiffuseMap.sampler = TextureList[0].textureSampler;

	//VkDescriptorImageInfo SpecularMap = {};
	//SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//SpecularMap.imageView = TextureList[1].textureImageView;
	//SpecularMap.sampler = TextureList[1].textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		std::array<WriteDescriptorSetInfo, 2>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		//WriteDescriptorInfo[2].DstBinding = 2;
		//WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		//WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		Mesh::CreateDescriptorSetsData(Renderer, std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Mesh::Draw(VulkanRenderer& Renderer, int currentFrame)
{
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = *GetRenderPass(Renderer);
		InheritanceInfo.framebuffer = Renderer.swapChainFramebuffers[i];

		VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
		BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

		vkBeginCommandBuffer(Renderer.SubCommandBuffers[i], &BeginSecondaryCommandBuffer);
		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline(Renderer.SubCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(Renderer));
		vkCmdBindVertexBuffers(Renderer.SubCommandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(Renderer.SubCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(Renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		if (indexBuffer == VK_NULL_HANDLE)
		{
			vkCmdDraw(Renderer.SubCommandBuffers[i], VertexSize, 1, 0, 0);
		}
		else
		{
			vkCmdBindIndexBuffer(Renderer.SubCommandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(Renderer.SubCommandBuffers[i], static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
		}
		if (vkEndCommandBuffer(Renderer.SubCommandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void Mesh::UpdateUniformBuffer(VulkanRenderer& Renderer, UniformBufferObject ubo2, int currentImage)
{
	BaseMesh::UpdateUniformBuffer(Renderer, uniformBuffersMemory[currentImage], static_cast<void*>(&ubo2), sizeof(ubo2));
}

void Mesh::Destroy(VulkanRenderer& Renderer)
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

