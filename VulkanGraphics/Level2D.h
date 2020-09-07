#pragma once
#include "LevelSprite.h"
#include "Sprite.h"
#include "DebugLightMesh.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Texture2D.h"
#include "Mesh2D.h"
#include "Light.h"
#include "LevelSprite.h"
#include "OrthographicCamera.h"
#include "GameManager.h"
enum Level
{
	SparkMan,
	Test
};

class Level2D
{
private:
	Keyboard keyboard;
	Mouse mouse;

	LevelSprite level;
	std::vector<std::shared_ptr<Sprite>> SpriteList;

	OrthographicCamera OrthoCamera;
	OrthographicCamera OrthoCamera2;

	Light light;
public:

	Level2D();
	Level2D(RendererManager& renderer, GameManager& gameManager);
	~Level2D();

	void LevelDebug(RendererManager& renderer);
	void LevelLoop(RendererManager& renderer, GLFWwindow* Window);
	void Update(RendererManager& renderer, OrthographicCamera& camera);
	void Draw(RendererManager& renderer, GLFWwindow* Window);
	void Destroy(RendererManager& renderer);
};

