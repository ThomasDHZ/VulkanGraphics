#pragma once
#include "BaseMesh.h"
#include "UniformBuffer.h"
#include "Mesh.h"
#include <map>

struct TileSet
{
	std::string DiffuseMap;
	std::string SpecularMap;
	std::string NormalMap;
	std::string DisplacementMap;
	std::string AlphaMap;
};

class LevelMesh2D
{
private:

	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	TextureMaps TextureList;
	Mesh LevelMesh;


	void LoadTiles(Renderer& renderer, const TileSet& tileSet);
	void CreateLevelGeometry();

public:

	LevelMesh2D();
	LevelMesh2D(Renderer& renderer, const TileSet& tileSet);
	~LevelMesh2D();

	void Update(Renderer& renderer, Camera& camera, Lights light);
	void Draw(Renderer& renderer, int currentFrame);
	void Destory(Renderer& renderer);
};