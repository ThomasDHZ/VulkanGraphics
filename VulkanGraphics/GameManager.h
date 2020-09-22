#pragma once
#include "RendererManager.h"
#include "VulkanEngine.h"
#include "TextureManager.h"
class GameManager
{
private:
public:

	std::shared_ptr<TextureManager> textureManager;
	RendererManager renderedr;

	GameManager();
	GameManager(VulkanEngine& renderer);
	~GameManager();
};

