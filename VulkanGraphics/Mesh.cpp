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
	ViewPosBuffer = UniformBuffer(Renderer, sizeof(ViewPos));
}

void Mesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 7>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo DiffuseMap = {};
	VkDescriptorImageInfo SpecularMap = {};
	if (TextureList.size() != 0)
	{
		DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		DiffuseMap.imageView = TextureList[0].textureImageView;
		DiffuseMap.sampler = TextureList[0].textureSampler;

		SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		SpecularMap.imageView = TextureList[1].textureImageView;
		SpecularMap.sampler = TextureList[1].textureSampler;
	}

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

		VkDescriptorBufferInfo ViewPosInfo = {};
		ViewPosInfo.buffer = ViewPosBuffer.GetUniformBuffer(i);
		ViewPosInfo.offset = 0;
		ViewPosInfo.range = sizeof(ViewPos);


		std::vector<WriteDescriptorSetInfo> DescriptorList;

		WriteDescriptorSetInfo PositionDescriptor;
		PositionDescriptor.DstBinding = 0;
		PositionDescriptor.DstSet = descriptorSets[i];
		PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		PositionDescriptor.DescriptorBufferInfo = PositionInfo;
		DescriptorList.emplace_back(PositionDescriptor);

		if (TextureList.size() != 0)
		{
			WriteDescriptorSetInfo DiffuseMapDescriptor;
			DiffuseMapDescriptor.DstBinding = 1;
			DiffuseMapDescriptor.DstSet = descriptorSets[i];
			DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			DiffuseMapDescriptor.DescriptorImageInfo = DiffuseMap;
			DescriptorList.emplace_back(DiffuseMapDescriptor);

			WriteDescriptorSetInfo SpecularMapDescriptor;
			SpecularMapDescriptor.DstBinding = 2;
			SpecularMapDescriptor.DstSet = descriptorSets[i];
			SpecularMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			SpecularMapDescriptor.DescriptorImageInfo = SpecularMap;
			DescriptorList.emplace_back(SpecularMapDescriptor);
		}

		WriteDescriptorSetInfo MaterialDescriptor;
		MaterialDescriptor.DstBinding = 3;
		MaterialDescriptor.DstSet = descriptorSets[i];
		MaterialDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		MaterialDescriptor.DescriptorBufferInfo = MarterialInfo;
		DescriptorList.emplace_back(MaterialDescriptor);

		WriteDescriptorSetInfo AmbiantLightDescriptor;
		AmbiantLightDescriptor.DstBinding = 4;
		AmbiantLightDescriptor.DstSet = descriptorSets[i];
		AmbiantLightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		AmbiantLightDescriptor.DescriptorBufferInfo = AmbiantLightInfo;
		DescriptorList.emplace_back(AmbiantLightDescriptor);

		WriteDescriptorSetInfo LightDescriptor;
		LightDescriptor.DstBinding = 5;
		LightDescriptor.DstSet = descriptorSets[i];
		LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		LightDescriptor.DescriptorBufferInfo = LightInfo;
		DescriptorList.emplace_back(LightDescriptor);

		WriteDescriptorSetInfo ViewPosDescriptor;
		ViewPosDescriptor.DstBinding = 6;
		ViewPosDescriptor.DstSet = descriptorSets[i];
		ViewPosDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		ViewPosDescriptor.DescriptorBufferInfo = ViewPosInfo;
		DescriptorList.emplace_back(ViewPosDescriptor);

		Mesh::CreateDescriptorSetsData(Renderer, DescriptorList);
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

void Mesh::UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix, AmbientLightUniformBuffer light, Lighter lighter, Material material, ViewPos viewpos)
{
	PositionMatrixBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&positionMatrix), Renderer.DrawFrame);
	MaterialBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&material), Renderer.DrawFrame);
	AmbientLightBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&light), Renderer.DrawFrame);
	LighterBuffers.UpdateUniformBuffer(Renderer, static_cast<void*>(&lighter), Renderer.DrawFrame);
	ViewPosBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&viewpos), Renderer.DrawFrame);
}

void Mesh::Destroy(VulkanRenderer& Renderer)
{

	PositionMatrixBuffer.Destroy(Renderer);
	MaterialBuffer.Destroy(Renderer);
	AmbientLightBuffer.Destroy(Renderer);
	LighterBuffers.Destroy(Renderer);
	ViewPosBuffer.Destroy(Renderer);

	BaseMesh::Destory(Renderer);
}

