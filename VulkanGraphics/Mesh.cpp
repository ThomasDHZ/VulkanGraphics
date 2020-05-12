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

Mesh::Mesh(VulkanRenderer& Renderer, const std::vector<Texture2D>& textureList) : BaseMesh(Renderer, textureList)
{

}

Mesh::~Mesh()
{
}

void Mesh::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(PositionMatrix);
	VkDeviceSize bufferSize2 = sizeof(AmbientLight);
	VkDeviceSize bufferSize3 = sizeof(Lighter);

	PositionMatrixUniformBuffers.resize(GetSwapChainImageCount(Renderer));
	PositionMatrixUniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, PositionMatrixUniformBuffers[i], PositionMatrixUniformBuffersMemory[i]);
	}

	MaterialUniformBuffers.resize(GetSwapChainImageCount(Renderer));
	MaterialUniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize2, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, MaterialUniformBuffers[i], MaterialUniformBuffersMemory[i]);
	}

	AmbientLightUniformBuffers.resize(GetSwapChainImageCount(Renderer));
	AmbientLightUniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize2, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, AmbientLightUniformBuffers[i], AmbientLightUniformBuffersMemory[i]);
	}

	LighterUniformBuffers.resize(GetSwapChainImageCount(Renderer));
	LighterUniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize3, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, LighterUniformBuffers[i], LighterUniformBuffersMemory[i]);
	}
}

void Mesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 6>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList[0].textureImageView;
	DiffuseMap.sampler = TextureList[0].textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureList[1].textureImageView;
	SpecularMap.sampler = TextureList[1].textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = PositionMatrixUniformBuffers[i];
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		VkDescriptorBufferInfo MarterialInfo = {};
		MarterialInfo.buffer = MaterialUniformBuffers[i];
		MarterialInfo.offset = 0;
		MarterialInfo.range = sizeof(Material);

		VkDescriptorBufferInfo AmbiantLightInfo = {};
		AmbiantLightInfo.buffer = AmbientLightUniformBuffers[i];
		AmbiantLightInfo.offset = 0;
		AmbiantLightInfo.range = sizeof(AmbientLightUniformBuffer);

		VkDescriptorBufferInfo LightInfo = {};
		LightInfo.buffer = LighterUniformBuffers[i];
		LightInfo.offset = 0;
		LightInfo.range = sizeof(Lighter);

		std::array<WriteDescriptorSetInfo, 6>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = PositionInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		WriteDescriptorInfo[3].DstBinding = 3;
		WriteDescriptorInfo[3].DstSet = descriptorSets[i];
		WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[3].DescriptorBufferInfo = MarterialInfo;

		WriteDescriptorInfo[4].DstBinding = 4;
		WriteDescriptorInfo[4].DstSet = descriptorSets[i];
		WriteDescriptorInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[4].DescriptorBufferInfo = AmbiantLightInfo;

		WriteDescriptorInfo[5].DstBinding = 5;
		WriteDescriptorInfo[5].DstSet = descriptorSets[i];
		WriteDescriptorInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[5].DescriptorBufferInfo = LightInfo;

		Mesh::CreateDescriptorSetsData(Renderer, std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Mesh::Draw(VulkanRenderer& Renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if(Renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetMeshViewShaderPipeline(Renderer));
	}
	else
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(Renderer));
	}

	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(Renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(Renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	if(IndiceSize == 0)
	{
		vkCmdDraw(*GetSecondaryCommandBuffer(Renderer, currentFrame), VertexSize, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(Renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*GetSecondaryCommandBuffer(Renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
	}
}

void Mesh::UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix, AmbientLightUniformBuffer light, Lighter lighter, int currentImage)
{
	Material material = {};
	material.Diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	material.Specular = glm::vec3(0.0f, 1.0f, 0.0f);
	BaseMesh::UpdateUniformBuffer(Renderer, PositionMatrixUniformBuffersMemory[currentImage], static_cast<void*>(&positionMatrix), sizeof(positionMatrix));
	BaseMesh::UpdateUniformBuffer(Renderer, MaterialUniformBuffersMemory[currentImage], static_cast<void*>(&material), sizeof(material));
	BaseMesh::UpdateUniformBuffer(Renderer, AmbientLightUniformBuffersMemory[currentImage], static_cast<void*>(&light), sizeof(light));
	BaseMesh::UpdateUniformBuffer(Renderer, LighterUniformBuffersMemory[currentImage], static_cast<void*>(&lighter), sizeof(lighter));
}

void Mesh::Destroy(VulkanRenderer& Renderer)
{
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		if (PositionMatrixUniformBuffers[i] != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(*GetDevice(Renderer), PositionMatrixUniformBuffers[i], nullptr);
			vkFreeMemory(*GetDevice(Renderer), PositionMatrixUniformBuffersMemory[i], nullptr);

			vkDestroyBuffer(*GetDevice(Renderer), MaterialUniformBuffers[i], nullptr);
			vkFreeMemory(*GetDevice(Renderer), MaterialUniformBuffersMemory[i], nullptr);

			vkDestroyBuffer(*GetDevice(Renderer), AmbientLightUniformBuffers[i], nullptr);
			vkFreeMemory(*GetDevice(Renderer), AmbientLightUniformBuffersMemory[i], nullptr);

			vkDestroyBuffer(*GetDevice(Renderer), LighterUniformBuffers[i], nullptr);
			vkFreeMemory(*GetDevice(Renderer), LighterUniformBuffersMemory[i], nullptr);


			PositionMatrixUniformBuffers[i] = VK_NULL_HANDLE;
			PositionMatrixUniformBuffersMemory[i] = VK_NULL_HANDLE;

			MaterialUniformBuffers[i] = VK_NULL_HANDLE;
			AmbientLightUniformBuffersMemory[i] = VK_NULL_HANDLE;

			AmbientLightUniformBuffers[i] = VK_NULL_HANDLE;
			AmbientLightUniformBuffersMemory[i] = VK_NULL_HANDLE;

			LighterUniformBuffers[i] = VK_NULL_HANDLE;
			LighterUniformBuffersMemory[i] = VK_NULL_HANDLE;
		}
	}

	BaseMesh::Destory(Renderer);
}

