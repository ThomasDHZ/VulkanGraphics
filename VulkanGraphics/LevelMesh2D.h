//#pragma once
//#include "BaseMesh.h"
//#include "UniformBuffer.h"
//#include "Mesh.h"
//#include <map>

#include "CubeMapTexture.h"
#include "Vertex.h"
#include <map>
#include "Mesh.h"

class LevelMesh2D 
{
private:
	CubeMapLayout layout;

	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;

	MeshTextures TextureList;


	void LoadTiles(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager);
	void CreateLevelGeometry(std::shared_ptr<TextureManager>textureManager);

public:


	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	Mesh LevelMesh;

	LevelMesh2D();
	LevelMesh2D(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, MeshTextures TileSet, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
	~LevelMesh2D();

	void Update(VulkanRenderer& renderer, Camera& camera, LightBufferObject light);
	void Destory(VulkanRenderer& renderer);
};