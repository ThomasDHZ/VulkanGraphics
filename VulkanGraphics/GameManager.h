#pragma once
#include "RendererManager.h"
#include "VulkanRenderer.h"
#include "TextureManager.h"
class GameManager
{
private:
public:

	std::shared_ptr<TextureManager> textureManager;
	RendererManager renderedr;

	GameManager();
	GameManager(RendererManager& renderer);
	~GameManager();
};

