#include "TileMesh.h"


LevelMesh2D::LevelMesh2D() : BaseMesh()
{
}

LevelMesh2D::LevelMesh2D(Renderer& renderer, const TileSet& tileSet) : BaseMesh(renderer)
{
	LoadTiles(renderer, tileSet);
	CreateLevelGeometry();
	CreateVertexBuffer(renderer);
	CreateIndiceBuffer(renderer);
	CreateUniformBuffers(renderer);
	CreateDescriptorPool(renderer);
	CreateDescriptorSets(renderer);
}

LevelMesh2D::~LevelMesh2D()
{
}

void LevelMesh2D::LoadTiles(Renderer& renderer, const TileSet& tileSet)
{
	TextureList.DiffuseMap = Texture2D(renderer, tileSet.DiffuseMap);
	TextureList.SpecularMap = Texture2D(renderer, tileSet.SpecularMap);
	TextureList.NormalMap = Texture2D(renderer, tileSet.NormalMap);
}

void LevelMesh2D::CreateLevelGeometry()
{
	const unsigned int TileSize = 2048;
	const float AmtXAxisTiles = TextureList.DiffuseMap.Width / TileSize;
	const float AmtYAxisTiles = TextureList.DiffuseMap.Height / TileSize;
	const float UVTileLocU = 1 / AmtXAxisTiles;
	const float UVTileLocV = 1 / AmtYAxisTiles;

	for (unsigned int x = 1; x < LevelBoundsX; x++)
	{
		for (unsigned int y = 1; y < LevelBoundsY; y++)
		{
			const unsigned int VertexCount = VertexList.size();
			const Vertex BottomLeftVertex = { {x, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * x, UVTileLocV * (y - 1)} };
			const Vertex BottomRightVertex = { {x + 1.0f, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (x - 1), UVTileLocV * (y - 1)} };
			const Vertex TopRightVertex = { {x + 1.0f, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (x - 1), UVTileLocV * y } };
			const Vertex TopLeftVertex = { {x, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * x, UVTileLocV * y} };

			VertexList.emplace_back(BottomLeftVertex);
			VertexList.emplace_back(BottomRightVertex);
			VertexList.emplace_back(TopRightVertex);
			VertexList.emplace_back(TopLeftVertex);

			IndexList.emplace_back(VertexCount);
			IndexList.emplace_back(VertexCount + 1);
			IndexList.emplace_back(VertexCount + 2);
			IndexList.emplace_back(VertexCount + 2);
			IndexList.emplace_back(VertexCount + 3);
			IndexList.emplace_back(VertexCount);
		}
	}

	VertexSize = VertexList.size();
	IndiceSize = IndexList.size();
}


void LevelMesh2D::CreateUniformBuffers(Renderer& renderer)
{
	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(PositionMatrix));
	ViewPosBuffer = UniformBuffer(renderer, sizeof(MeshProp));
}

void LevelMesh2D::CreateDescriptorPool(Renderer& renderer)
{
	std::array<DescriptorPoolSizeInfo, 4>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void LevelMesh2D::CreateDescriptorSets(Renderer& renderer)
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

		WriteDescriptorSetInfo ViewPosDescriptor;
		ViewPosDescriptor.DstBinding = 3;
		ViewPosDescriptor.DstSet = descriptorSets[i];
		ViewPosDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		ViewPosDescriptor.DescriptorBufferInfo = ViewPosInfo;
		DescriptorList.emplace_back(ViewPosDescriptor);

		BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
	}
}

void LevelMesh2D::Draw(Renderer& renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if (renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetMeshViewShaderPipeline(renderer));
	}
	else
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(renderer));
	}

	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
}

void LevelMesh2D::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos)
{
	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
	ViewPosBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&viewpos));
}

void LevelMesh2D::Destroy(Renderer& renderer)
{

	PositionMatrixBuffer.Destroy(renderer);
	ViewPosBuffer.Destroy(renderer);

	BaseMesh::Destory(renderer);
}

