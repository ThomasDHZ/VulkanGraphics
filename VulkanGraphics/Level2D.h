//#pragma once
//#include <string>
//#include "Texture2D.h"
//#include <map>
//#include "Camera.h"
//#include "LightManager.h"
//#include "GUIDebugger.h"
//#include "LevelMesh2D.h"
//#include "Sprite.h"
//#include <chrono>
//#include "MegaMan.h"
//#include "MMShot.h"
//
//enum Level
//{
//	SparkMan,
//	Test
//};
//
//class Level2D : VulkanResources
//{
//private:
//	LevelMesh2D LevelMap;
//	std::vector<Sprite> SpriteList;
//
//	Lights light = {};
//public:
//	Camera camera;
//
//	Level2D();
//	Level2D(Renderer& renderer, TileSet tileset);
//	~Level2D();
//
//	void LevelDebug(Renderer& renderer);
//	void LevelLoop(Renderer& renderer);
//	void Update(Renderer& renderer, GLFWwindow* Window);
//	void Draw(Renderer& renderer, uint32_t DrawFrame);
//	void Destroy(Renderer& renderer);
//};
//
