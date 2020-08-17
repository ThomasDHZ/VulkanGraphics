//#pragma once
//#include "BaseMesh.h"
//#include "UniformBuffer.h"
//#include "Mesh.h"
//#include <map>

#include "CubeMapTexture.h"
#include "Vertex.h"
#include <map>
#include "Mesh.h"

struct TileSet
{
	std::string DiffuseMap;
	std::string SpecularMap;
	std::string NormalMap;
	std::string DisplacementMap;
	std::string EmissionMap;
	std::string AlphaMap;
};

class LevelMesh2D
{
private:
	CubeMapLayout layout;

	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	MeshTextures TextureList;
	Mesh LevelMesh;


	void LoadTiles(VulkanRenderer& renderer, const TileSet& tileSet);
	void CreateLevelGeometry();

public:

	LevelMesh2D();
	LevelMesh2D(VulkanRenderer& renderer, const TileSet& tileSet);
	~LevelMesh2D();

	void Update(VulkanRenderer& renderer, Camera& camera);
	void Draw(VulkanRenderer& renderer, int currentFrame);
	void Destory(VulkanRenderer& renderer);
};