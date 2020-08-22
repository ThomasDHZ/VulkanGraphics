#include "LevelMesh2D.h"


LevelMesh2D::LevelMesh2D()
{
}

LevelMesh2D::LevelMesh2D(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures TileSet, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit)
{
	LoadTiles(renderer, textureManager);
	CreateLevelGeometry(textureManager);
	LevelMesh = Mesh(renderer, textureManager, VertexList, IndexList, TileSet, cubemap, descriptorSetLayout, renderBit);
}

LevelMesh2D::~LevelMesh2D()
{
}

void LevelMesh2D::LoadTiles(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager)
{
	const unsigned int TileSize = 16;
	const float AmtXAxisTiles = textureManager->GetTextureByID(LevelMesh.DiffuseMapID).Width / TileSize;
	const float AmtYAxisTiles = textureManager->GetTextureByID(LevelMesh.DiffuseMapID).Height / TileSize;
	const float UVTileLocU = 1 / AmtXAxisTiles;
	const float UVTileLocV = 1 / AmtYAxisTiles;

	for (int x = 0; x <= AmtXAxisTiles; x++)
	{
		for (int y = 0; y < AmtYAxisTiles; y++)
		{
			TileMap[x + (y * AmtXAxisTiles)] = glm::ivec2(x + 1, y);
		}
	}
}

void LevelMesh2D::CreateLevelGeometry(std::shared_ptr<TextureManager> textureManager)
{
	const unsigned int TileSize = 16;
	const float AmtXAxisTiles = textureManager->GetTextureByID(LevelMesh.DiffuseMapID).Width / TileSize;
	const float AmtYAxisTiles = textureManager->GetTextureByID(LevelMesh.DiffuseMapID).Height / TileSize;
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
			const Vertex BottomLeftVertex = { {x, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex BottomRightVertex = { {x + 1.0f, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex TopRightVertex = { {x + 1.0f, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y }, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex TopLeftVertex = { {x, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };

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
}

void LevelMesh2D::Update(VulkanRenderer& renderer, Camera& camera, LightBufferObject light)
{
	LevelMesh.Update(renderer, camera, light);
}

void LevelMesh2D::Destory(VulkanRenderer& renderer)
{
	LevelMesh.Destory(renderer);
}

