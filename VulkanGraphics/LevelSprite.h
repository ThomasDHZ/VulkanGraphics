#pragma once
#include "Mesh2D.h"
#include "RendererManager.h"

class LevelSprite
{
private:
	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	std::shared_ptr<Mesh2D> LevelMesh;

	void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
public:
	LevelSprite();
	LevelSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
	~LevelSprite();

	void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer);
};

