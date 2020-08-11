#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::GameManager(VulkanRenderer& renderer)
{
	textureManager = std::make_unique<TextureManager>(renderer);
}

GameManager::~GameManager()
{
}
