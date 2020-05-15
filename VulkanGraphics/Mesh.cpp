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
	PositionMatrixBuffer = UniformBuffer(Renderer, sizeof(PositionMatrix));
	MaterialBuffer = UniformBuffer(Renderer, sizeof(Material));
	AmbientLightBuffer = UniformBuffer(Renderer, sizeof(AmbientLight));
	LighterBuffers = UniformBuffer(Renderer, sizeof(Lighter));
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
		PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		VkDescriptorBufferInfo MarterialInfo = {};
		MarterialInfo.buffer = MaterialBuffer.GetUniformBuffer(i);
		MarterialInfo.offset = 0;
		MarterialInfo.range = sizeof(Material);

		VkDescriptorBufferInfo AmbiantLightInfo = {};
		AmbiantLightInfo.buffer = AmbientLightBuffer.GetUniformBuffer(i);
		AmbiantLightInfo.offset = 0;
		AmbiantLightInfo.range = sizeof(AmbientLightUniformBuffer);

		VkDescriptorBufferInfo LightInfo = {};
		LightInfo.buffer = LighterBuffers.GetUniformBuffer(i);
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
	//light.objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	//light.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	//light.lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
	//light.viewPos = glm::vec3(0.0f, 0.0f, 3.0f);

	PositionMatrixBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&positionMatrix), currentImage);
	MaterialBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&material), currentImage);
	AmbientLightBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&light), currentImage);
	LighterBuffers.UpdateUniformBuffer(Renderer, static_cast<void*>(&lighter), currentImage);
}

void Mesh::Destroy(VulkanRenderer& Renderer)
{

	PositionMatrixBuffer.Destroy(Renderer);
	MaterialBuffer.Destroy(Renderer);
	AmbientLightBuffer.Destroy(Renderer);
	LighterBuffers.Destroy(Renderer);

	BaseMesh::Destory(Renderer);
}

