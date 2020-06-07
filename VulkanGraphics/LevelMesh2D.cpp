#include "LevelMesh2D.h"


LevelMesh2D::LevelMesh2D() : Mesh()
{
}

LevelMesh2D::LevelMesh2D(Renderer& renderer, const TileSet& tileSet) : Mesh(renderer)
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
	TextureList.AlphaMap = Texture2D(renderer, tileSet.AlphaMap);

	TileMap[0] = glm::ivec2(1, 0);
	TileMap[1] = glm::ivec2(2, 0);
	TileMap[2] = glm::ivec2(3, 0);
	TileMap[3] = glm::ivec2(4, 0);
	TileMap[4] = glm::ivec2(5, 0);
	TileMap[5] = glm::ivec2(6, 0);
	TileMap[6] = glm::ivec2(7, 0);
	TileMap[7] = glm::ivec2(8, 0);
	TileMap[8] = glm::ivec2(9, 0);
	TileMap[9] = glm::ivec2(10, 0);
	TileMap[10] = glm::ivec2(11, 0);
	TileMap[11] = glm::ivec2(12, 0);
	TileMap[12] = glm::ivec2(13, 0);
	TileMap[13] = glm::ivec2(14, 0);
	TileMap[14] = glm::ivec2(15, 0);
	TileMap[15] = glm::ivec2(16, 0);
	TileMap[16] = glm::ivec2(1, 2);
	TileMap[17] = glm::ivec2(2, 2);
	TileMap[18] = glm::ivec2(3, 2);
	TileMap[19] = glm::ivec2(4, 2);
	TileMap[20] = glm::ivec2(5, 2);
	TileMap[21] = glm::ivec2(6, 2);
	TileMap[22] = glm::ivec2(7, 2);
	TileMap[23] = glm::ivec2(8, 2);
	TileMap[24] = glm::ivec2(9, 2);
	TileMap[25] = glm::ivec2(10, 2);
	TileMap[26] = glm::ivec2(11, 2);
	TileMap[27] = glm::ivec2(12, 2);
	TileMap[28] = glm::ivec2(13, 2);
	TileMap[29] = glm::ivec2(14, 2);
	TileMap[30] = glm::ivec2(15, 2);
	TileMap[31] = glm::ivec2(16, 2);
}

void LevelMesh2D::CreateLevelGeometry()
{
	const unsigned int TileSize = 2048;
	const float AmtXAxisTiles = TextureList.DiffuseMap.Width / TileSize;
	const float AmtYAxisTiles = TextureList.DiffuseMap.Height / TileSize;
	const float UVTileLocU = 1 / AmtXAxisTiles;
	const float UVTileLocV = 1 / AmtYAxisTiles;

	MapLocs = { 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
				5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
				5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
				13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 5, };

	for (unsigned int x = 1; x < LevelBoundsX; x++)
	{
		for (unsigned int y = 1; y < LevelBoundsY; y++)
		{
			const unsigned int VertexCount = VertexList.size();
			const Vertex BottomLeftVertex = { {x, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)} };
			const Vertex BottomRightVertex = { {x + 1.0f, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)} };
			const Vertex TopRightVertex = { {x + 1.0f, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y } };
			const Vertex TopLeftVertex = { {x, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y} };

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

void LevelMesh2D::Destory(Renderer& renderer)
{
	TextureList.DiffuseMap.Destroy(renderer);
	TextureList.SpecularMap.Destroy(renderer);
	TextureList.NormalMap.Destroy(renderer);
	TextureList.AlphaMap.Destroy(renderer);
	Mesh::Destroy(renderer);
}

