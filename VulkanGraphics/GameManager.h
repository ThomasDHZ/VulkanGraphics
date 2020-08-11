#pragma once
#include "VulkanRenderer.h"
#include "TextureManager.h"
class GameManager
{
private:
public:

	std::shared_ptr<TextureManager> textureManager;

	GameManager();
	GameManager(VulkanRenderer& renderer);
	~GameManager();
};

