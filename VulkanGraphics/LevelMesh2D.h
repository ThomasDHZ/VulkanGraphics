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
	std::string AlphaMap;
};

class LevelMesh2D : public Mesh
{
private:

	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 8;

	std::map<int, glm::ivec2> TileMap;
	std::vector<int> MapLocs;


	void LoadTiles(Renderer& renderer, const TileSet& tileSet);
	void CreateLevelGeometry();

public:

	LevelMesh2D();
	LevelMesh2D(Renderer& renderer, const TileSet& tileSet);
	~LevelMesh2D();

	void Draw(Renderer& renderer, int currentFrame);
};