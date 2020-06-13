#include "Mesh.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(Renderer& renderer) : BaseMesh(renderer)
{
}

Mesh::Mesh(Renderer& renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, const TextureMaps& textureList) : BaseMesh(renderer, vertexList, indexList, textureList)
{
	CreateUniformBuffers(renderer);
	CreateDescriptorPool(renderer);
	CreateDescriptorSets(renderer);
}

Mesh::Mesh(Renderer& renderer, const TextureMaps& textureList) : BaseMesh(renderer, textureList)
{

}

Mesh::~Mesh()
{
}

void Mesh::CreateUniformBuffers(Renderer& renderer)
{
	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(PositionMatrix));
	ViewPosBuffer = UniformBuffer(renderer, sizeof(MeshProp));
}

void Mesh::CreateDescriptorPool(Renderer& renderer)
{
	std::array<DescriptorPoolSizeInfo, 7>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(Renderer& renderer)
{
	BaseMesh::CreateDescriptorSets(renderer, *GetDescriptorSetLayout(renderer));

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList.DiffuseMap.textureImageView;
	DiffuseMap.sampler = TextureList.DiffuseMap.textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureList.SpecularMap.textureImageView;
	SpecularMap.sampler = TextureList.SpecularMap.textureSampler;

	VkDescriptorImageInfo NormalMap = {};
	NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	NormalMap.imageView = TextureList.NormalMap.textureImageView;
	NormalMap.sampler = TextureList.NormalMap.textureSampler;

	VkDescriptorImageInfo DisplacementMap = {};
	DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DisplacementMap.imageView = TextureList.DisplacementMap.textureImageView;
	DisplacementMap.sampler = TextureList.DisplacementMap.textureSampler;

	VkDescriptorImageInfo AlphaMap = {};
	AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	AlphaMap.imageView = TextureList.AlphaMap.textureImageView;
	AlphaMap.sampler = TextureList.AlphaMap.textureSampler;

	VkDescriptorImageInfo CubeMap = {};
	CubeMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	CubeMap.imageView = TextureList.CubeMap.textureImageView;
	CubeMap.sampler = TextureList.CubeMap.textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
	{
		VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		VkDescriptorBufferInfo ViewPosInfo = {};
		ViewPosInfo.buffer = ViewPosBuffer.GetUniformBuffer(i);
		ViewPosInfo.offset = 0;
		ViewPosInfo.range = sizeof(MeshProp);


		std::vector<WriteDescriptorSetInfo> DescriptorList;

		WriteDescriptorSetInfo PositionDescriptor;
		PositionDescriptor.DstBinding = 0;
		PositionDescriptor.DstSet = descriptorSets[i];
		PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		PositionDescriptor.DescriptorBufferInfo = PositionInfo;
		DescriptorList.emplace_back(PositionDescriptor);

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

		WriteDescriptorSetInfo NormalMapDescriptor;
		NormalMapDescriptor.DstBinding = 3;
		NormalMapDescriptor.DstSet = descriptorSets[i];
		NormalMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		NormalMapDescriptor.DescriptorImageInfo = NormalMap;
		DescriptorList.emplace_back(NormalMapDescriptor);

		WriteDescriptorSetInfo DisplacementMapDescriptor;
		DisplacementMapDescriptor.DstBinding = 4;
		DisplacementMapDescriptor.DstSet = descriptorSets[i];
		DisplacementMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DisplacementMapDescriptor.DescriptorImageInfo = DisplacementMap;
		DescriptorList.emplace_back(DisplacementMapDescriptor);

		WriteDescriptorSetInfo AlphaMapDescriptor;
		AlphaMapDescriptor.DstBinding = 5;
		AlphaMapDescriptor.DstSet = descriptorSets[i];
		AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
		DescriptorList.emplace_back(AlphaMapDescriptor);

		WriteDescriptorSetInfo CubeMapDescriptor;
		CubeMapDescriptor.DstBinding = 6;
		CubeMapDescriptor.DstSet = descriptorSets[i];
		CubeMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		CubeMapDescriptor.DescriptorImageInfo = CubeMap;
		DescriptorList.emplace_back(CubeMapDescriptor);

		WriteDescriptorSetInfo ViewPosDescriptor;
		ViewPosDescriptor.DstBinding = 7;
		ViewPosDescriptor.DstSet = descriptorSets[i];
		ViewPosDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		ViewPosDescriptor.DescriptorBufferInfo = ViewPosInfo;
		DescriptorList.emplace_back(ViewPosDescriptor);

		Mesh::CreateDescriptorSetsData(renderer, DescriptorList);
	}
}

void Mesh::Draw(Renderer& renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if(renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetMeshViewShaderPipeline(renderer));
		vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		if (IndiceSize == 0)
		{
			vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
		}
		else
		{
			vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
		}
	}

	if (renderer.Settings.ShowDebugCollisionMesh)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetCollisionShaderPipeline(renderer));
		vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		if (IndiceSize == 0)
		{
			vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
		}
		else
		{
			vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
		}
	}

	vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(renderer));
	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	if(IndiceSize == 0)
	{
		vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
	}
}

void Mesh::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos)
{
	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
	ViewPosBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&viewpos));
}

void Mesh::Destroy(Renderer& renderer)
{
	PositionMatrixBuffer.Destroy(renderer);
	ViewPosBuffer.Destroy(renderer);

	BaseMesh::Destory(renderer);
}

