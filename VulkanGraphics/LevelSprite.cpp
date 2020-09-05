#include "LevelSprite.h"

LevelSprite::LevelSprite()
{
}

LevelSprite::LevelSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures, VkDescriptorSetLayout& descriptorSetLayout, int renderBit)
{
	LoadTiles(renderer, textureManager, textures);
	LevelMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, VertexList, IndexList, textures, renderBit));

	//DrawMessage draw = {};
	//draw.RenderBit = RendererBitFlag::RenderOnMainPass;
	//draw.pipeline = renderer.forwardRenderer.renderer2DPipeline;
	//draw.mesh = LevelMesh;
	//auto a = std::make_shared<DrawMessage>(draw);
	////renderer.AddDrawableMesh(a);
	//draw.RenderBit = RendererBitFlag::RenderOnTexturePass;
	//draw.pipeline = renderer.forwardRenderer.renderer2DPipeline;
	//draw.mesh = LevelMesh;
	//renderer.AddDrawableMesh(a);
	renderer.AddDrawableMesh(LevelMesh);

	DrawMeshMessage mainDraw = {};
	mainDraw.RendererID = 1;
	mainDraw.ObjectMesh = LevelMesh;
	mainDraw.pipeline = renderer.forwardRenderer.renderer2DPipeline;

	renderer.DrawMessageList.emplace_back(mainDraw);
}

LevelSprite::~LevelSprite()
{
}

void LevelSprite::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer)
{
	LevelMesh->Update(renderer, camera, Lightbuffer);
}

void LevelSprite::Destory(RendererManager& renderer)
{
	LevelMesh->Destory(renderer);
}

void LevelSprite::LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures)
{
	unsigned int DiffuseMapID = textureManager->LoadTexture(renderer, textures.DiffuseMap, VK_FORMAT_R8G8B8A8_SRGB);

	const unsigned int TileSize = 16;
	const float AmtXAxisTiles = textureManager->GetTextureByID(DiffuseMapID).Width / TileSize;
	const float AmtYAxisTiles = textureManager->GetTextureByID(DiffuseMapID).Height / TileSize;
	const float UVTileLocU = 1 / AmtXAxisTiles;
	const float UVTileLocV = 1 / AmtYAxisTiles;

	for (int x = 0; x <= AmtXAxisTiles; x++)
	{
		for (int y = 0; y < AmtYAxisTiles; y++)
		{
			TileMap[x + (y * AmtXAxisTiles)] = glm::ivec2(x + 1, y);
		}
	}

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
			const Vertex BottomLeftVertex = { {x, y, -0.01f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex BottomRightVertex = { {x + 1.0f, y, -0.01f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex TopRightVertex = { {x + 1.0f, y + 1.0f, -0.01f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y }, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
			const Vertex TopLeftVertex = { {x, y + 1.0f, -0.01f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };

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


			LevelColliderList.push_back(BoxCollider(TopLeftVertex.Position.x, TopRightVertex.Position.x, TopRightVertex.Position.y, BottomRightVertex.Position.y));
		}
	}

	int a = 34;
}
