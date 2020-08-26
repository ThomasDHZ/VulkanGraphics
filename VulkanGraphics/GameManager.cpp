#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::GameManager(RendererManager& renderer)
{
	textureManager = std::make_unique<TextureManager>(renderer);
}

GameManager::~GameManager()
{
}
