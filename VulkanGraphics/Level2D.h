#pragma once
#include <string>
#include "Texture2D.h"
#include <map>
#include "Camera.h"
#include "LightManager.h"
#include "GUIDebugger.h"
#include "LevelMesh2D.h"
#include "Sprite.h"
#include <chrono>

enum Level
{
	SparkMan,
	Test
};

const std::vector<Vertex> MegaManVertices =
{
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
};

const std::vector<uint16_t> MegaManIndices =
{
	  0, 1, 2, 2, 3, 0
};

class Level2D : VulkanResources
{
private:
public:
	bool tempflag = false;
	Camera camera;
	LevelMesh2D LevelMap;
	LightManager lightManager;

	Mesh ColliderSprite;
	Sprite SpriteList;

	Level2D();
	Level2D(Renderer& renderer, TileSet tileset);
	~Level2D();

	void LevelDebug(Renderer& renderer);
	void LevelLoop(Renderer& renderer);
	void Update(Renderer& renderer, GLFWwindow* Window);
	void Draw(Renderer& renderer, uint32_t DrawFrame);
	void Destroy(Renderer& renderer);
};

