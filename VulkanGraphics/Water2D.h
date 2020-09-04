#pragma once
#include "Sprite.h"
class Water2D : public Sprite
{
private:

	Animation2D CoinTurn;
public:
	Water2D();
	Water2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize);
	Water2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize, Texture& texture);
	~Water2D();

	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

