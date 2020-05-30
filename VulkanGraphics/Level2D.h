#pragma once
#include <string>
#include "Texture2D.h"
#include <map>
#include "Mesh.h"
#include "Camera.h"
#include "LightManager.h"
#include "GUIDebugger.h"

enum Level
{
	SparkMan,
	Test
};

struct TileSet
{
	std::string DiffuseMap;
	std::string SpecularMap;
	std::string NormalMap;
};

const std::vector<Vertex> vertices =
{
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.06666f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.06666f, 0.5f}}
};

const std::vector<uint16_t> indices =
{
	  0, 1, 2, 2, 3, 0
};

class Level2D : VulkanResources
{
private:
public:
	
	Camera camera;
	LightManager lightManager;
	
	Texture2D DiffuseMap;
	Texture2D SpecularMap;
	Texture2D NormalMap;

	unsigned int LevelBoundsX = 2;
	unsigned int LevelBoundsY = 2;
	std::vector<Mesh> TileList;

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

