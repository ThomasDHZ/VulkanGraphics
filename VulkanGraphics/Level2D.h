#pragma once
#include <string>
#include "Texture2D.h"
#include <map>
#include "Camera.h"
#include "LightManager.h"
#include "GUIDebugger.h"
#include "LevelMesh2D.h"

enum Level
{
	SparkMan,
	Test
};

const std::vector<Vertex> MegaManVertices = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	{{0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
{ {0.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  1.0f }},
{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  1.0f }},

{ {0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  1.0f }},
{ {1.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  0.0f }}
};

class Level2D : VulkanResources
{
private:
public:
	
	Camera camera;
	LevelMesh2D LevelMap;
	LightManager lightManager;

	Mesh MeshList;

	Level2D();
	Level2D(Renderer& renderer, TileSet tileset);
	~Level2D();

	void LevelDebug(Renderer& renderer);
	void LevelLoop(Renderer& renderer);
	void Update(Renderer& renderer);
	void Draw(Renderer& renderer, uint32_t DrawFrame);
	void PerFrameDraw(Renderer& renderer, uint32_t DrawFrame);
	void Destroy(Renderer& renderer);
};

