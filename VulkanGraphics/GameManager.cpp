#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::GameManager(VulkanEngine& renderer)
{
	textureManager = std::make_unique<TextureManager>(renderer);
}

GameManager::~GameManager()
{
}
