#pragma once
#include "BaseMesh.h"
#include "UniformBuffer.h"
#include "Mesh.h"

struct TileSet
{
	std::string DiffuseMap;
	std::string SpecularMap;
	std::string NormalMap;
};

class LevelMesh2D : public BaseMesh
{
private:

	unsigned int LevelBoundsX = 16;
	unsigned int LevelBoundsY = 3;

	void LoadTiles(Renderer& renderer, const TileSet& tileSet);
	void CreateLevelGeometry();
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);

public:
	UniformBuffer PositionMatrixBuffer;
	UniformBuffer ViewPosBuffer;

	LevelMesh2D();
	LevelMesh2D(Renderer& renderer, const TileSet& tileSet);
	~LevelMesh2D();

	void Draw(Renderer& renderer, int currentFrame);
	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos);
	void Destroy(Renderer& renderer);
};