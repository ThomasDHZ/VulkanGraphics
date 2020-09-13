#pragma once
#include "Object2D.h"

class LevelSprite : public Object2D
{
private:
	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
public:
	LevelSprite();
	LevelSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
	~LevelSprite();
};

