//#include "LevelMesh2D.h"
//
//
//LevelMesh2D::LevelMesh2D()
//{
//}
//
//LevelMesh2D::LevelMesh2D(Renderer& renderer, const TileSet& tileSet)
//{
//	LoadTiles(renderer, tileSet);
//	CreateLevelGeometry();
//	LevelMesh = Mesh(renderer, VertexList, IndexList, TextureList);
//}
//
//LevelMesh2D::~LevelMesh2D()
//{
//}
//
//void LevelMesh2D::LoadTiles(Renderer& renderer, const TileSet& tileSet)
//{
//	layout.Left = "texture/skybox/left.jpg";
//	layout.Right = "texture/skybox/right.jpg";
//	layout.Top = "texture/skybox/top.jpg";
//	layout.Bottom = "texture/skybox/bottom.jpg";
//	layout.Back = "texture/skybox/back.jpg";
//	layout.Front = "texture/skybox/front.jpg";
//
//	TextureList.DiffuseMap = Texture2D(renderer, tileSet.DiffuseMap);
//	TextureList.SpecularMap = Texture2D(renderer, tileSet.SpecularMap);
//	TextureList.NormalMap = Texture2D(renderer, tileSet.NormalMap);
//	TextureList.DisplacementMap = Texture2D(renderer, tileSet.AlphaMap);
//	TextureList.EmissionMap = Texture2D(renderer, tileSet.EmissionMap);
//	TextureList.AlphaMap = Texture2D(renderer, tileSet.AlphaMap);
//	TextureList.CubeMap = CubeMapTexture(renderer, layout);
//
//	const unsigned int TileSize = 16;
//	const float AmtXAxisTiles = TextureList.DiffuseMap.Width / TileSize;
//	const float AmtYAxisTiles = TextureList.DiffuseMap.Height / TileSize;
//	const float UVTileLocU = 1 / AmtXAxisTiles;
//	const float UVTileLocV = 1 / AmtYAxisTiles;
//
//	for (int x = 0; x <= AmtXAxisTiles; x++)
//	{
//		for (int y = 0; y < AmtYAxisTiles; y++)
//		{
//			TileMap[x + (y * AmtXAxisTiles)] = glm::ivec2(x + 1, y);
//		}
//	}
//}
//
//void LevelMesh2D::CreateLevelGeometry()
//{
//	const unsigned int TileSize = 16;
//	const float AmtXAxisTiles = TextureList.DiffuseMap.Width / TileSize;
//	const float AmtYAxisTiles = TextureList.DiffuseMap.Height / TileSize;
//	const float UVTileLocU = 1 / AmtXAxisTiles;
//	const float UVTileLocV = 1 / AmtYAxisTiles;
//
//	MapLocs = { 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
//				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
//				5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
//				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
//				5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6,
//				3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,
//				13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 5, };
//
//	for (unsigned int x = 1; x < LevelBoundsX; x++)
//	{
//		for (unsigned int y = 1; y < LevelBoundsY; y++)
//		{
//			const unsigned int VertexCount = VertexList.size();
//			const Vertex BottomLeftVertex = { {x, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
//			const Vertex BottomRightVertex = { {x + 1.0f, y, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y - 1)}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
//			const Vertex TopRightVertex = { {x + 1.0f, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * (TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x - 1), UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y }, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
//			const Vertex TopLeftVertex = { {x, y + 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {UVTileLocU * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].x, UVTileLocV * TileMap[MapLocs[x + ((y - 1) * LevelBoundsX)]].y}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} };
//
//			VertexList.emplace_back(BottomLeftVertex);
//			VertexList.emplace_back(BottomRightVertex);
//			VertexList.emplace_back(TopRightVertex);
//			VertexList.emplace_back(TopLeftVertex);
//
//			IndexList.emplace_back(VertexCount);
//			IndexList.emplace_back(VertexCount + 1);
//			IndexList.emplace_back(VertexCount + 2);
//			IndexList.emplace_back(VertexCount + 2);
//			IndexList.emplace_back(VertexCount + 3);
//			IndexList.emplace_back(VertexCount);
//		}
//	}
//}
//
//void LevelMesh2D::Update(Renderer& renderer, Camera& camera, Lights light)
//{
//	LevelMesh.Update(renderer, camera, light);
//}
//
//void LevelMesh2D::Draw(Renderer& renderer, int currentFrame)
//{
//	LevelMesh.Draw(renderer, currentFrame);
//}
//
//void LevelMesh2D::Destory(Renderer& renderer)
//{
//	TextureList.DiffuseMap.Destroy(renderer);
//	TextureList.SpecularMap.Destroy(renderer);
//	TextureList.NormalMap.Destroy(renderer);
//	TextureList.DisplacementMap.Destroy(renderer);
//	TextureList.AlphaMap.Destroy(renderer);
//	TextureList.CubeMap.Destroy(renderer);
//	LevelMesh.Destroy(renderer);
//}
//
