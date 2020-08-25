#pragma once
#include <string>
#include "Texture2D.h"
#include <map>
#include "Camera.h"
#include "GUIDebugger.h"
#include "LevelMesh2D.h"
#include "Sprite.h"
#include <chrono>
#include "MegaMan.h"
#include "MMShot.h"

enum Level
{
	SparkMan,
	Test
};

class Level2D
{
private:

public:

	Level2D();
	Level2D(VulkanRenderer& renderer);
	~Level2D();

	void LevelDebug(VulkanRenderer& renderer);
	void LevelLoop(VulkanRenderer& renderer);
	void Update(VulkanRenderer& renderer, GLFWwindow* Window);
	void Draw(VulkanRenderer& renderer, uint32_t DrawFrame);
	void Destroy(VulkanRenderer& renderer);
};

