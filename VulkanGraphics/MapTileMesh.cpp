#include "MapTileMesh.h"

MapTileMesh::MapTileMesh() : BaseMesh()
{
}

MapTileMesh::MapTileMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, const std::vector<Texture2D>& textureList) : BaseMesh(Renderer, vertexList, indexList, textureList)
{
	CreateUniformBuffers(Renderer);
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer);
}

MapTileMesh::~MapTileMesh()
{
}

void MapTileMesh::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer = UniformBuffer(Renderer, sizeof(PositionMatrix));
}

void MapTileMesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void MapTileMesh::CreateDescriptorSets(VulkanRenderer& Renderer)
{
	BaseMesh::CreateDescriptorSets(Renderer, *GetDescriptorSetLayout(Renderer));

	VkDescriptorImageInfo MapTile = {};
	MapTile.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	MapTile.imageView = TextureList[0].textureImageView;
	MapTile.sampler = TextureList[0].textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		std::vector<WriteDescriptorSetInfo> DescriptorList;

		WriteDescriptorSetInfo PositionDescriptor;
		PositionDescriptor.DstBinding = 0;
		PositionDescriptor.DstSet = descriptorSets[i];
		PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		PositionDescriptor.DescriptorBufferInfo = PositionInfo;
		DescriptorList.emplace_back(PositionDescriptor);

		WriteDescriptorSetInfo MapTileDescriptor;
		MapTileDescriptor.DstBinding = 1;
		MapTileDescriptor.DstSet = descriptorSets[i];
		MapTileDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		MapTileDescriptor.DescriptorImageInfo = MapTile;
		DescriptorList.emplace_back(MapTileDescriptor);

		MapTileMesh::CreateDescriptorSetsData(Renderer, DescriptorList);
	}
}

void MapTileMesh::Draw(VulkanRenderer& Renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if (Renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetMeshViewShaderPipeline(Renderer));
	}
	else
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(Renderer));
	}

	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(Renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(Renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(Renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(*GetSecondaryCommandBuffer(Renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
}

void MapTileMesh::UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix)
{
	PositionMatrixBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&positionMatrix));
}

void MapTileMesh::Destroy(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer.Destroy(Renderer);
	BaseMesh::Destory(Renderer);
}

