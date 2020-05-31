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
public:
	
	Camera camera;
	LevelMesh2D LevelMap;
	LightManager lightManager;

	Level2D();
	Level2D(Renderer& renderer, TileSet tileset);
	~Level2D();

	void LevelDebug(Renderer& renderer);
	void LevelLoop(Renderer& renderer);
	void Update(Renderer& renderer);
	void Draw(Renderer& renderer, uint32_t DrawFrame);
	void Destroy(Renderer& renderer);
};

