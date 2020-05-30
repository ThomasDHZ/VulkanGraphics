#pragma once
#include <string>
#include "Texture2D.h"
#include <map>
#include "Camera.h"
#include "LightManager.h"
#include "GUIDebugger.h"
#include "TileMesh.h"

enum Level
{
	SparkMan,
	Test
};

class Level2D : VulkanResources
{
private:
	std::vector<Vertex> LevelVertexList;
	std::vector<uint16_t> LevelIndiceList;

	void CreateLevelGeometry();
public:
	
	Camera camera;
	LightManager lightManager;
	
	Texture2D DiffuseMap;
	Texture2D SpecularMap;
	Texture2D NormalMap;

	unsigned int LevelBoundsX = 100;
	unsigned int LevelBoundsY = 200;
	LevelMesh2D LevelMap;

	Level2D();
	Level2D(Renderer& renderer, TileSet tileset);
	~Level2D();

	void LevelDebug(Renderer& renderer);
	void LevelLoop(Renderer& renderer);
	void Update(Renderer& renderer);
	void Draw(Renderer& renderer, uint32_t DrawFrame);
	void LoadTileSet(Renderer& renderer, TileSet tileset);
	void UnloadTileSet(Renderer& renderer);
	void Destroy(Renderer& renderer);
};

