#pragma once
#include "Sprite.h"
#include "TextureRenderer.h"
class Coin : public Sprite
{
private:

	static constexpr glm::vec2 SpriteSize = glm::vec2(1.0f, 1.0f);
	Animation2D CoinTurn;
public:
	Coin();
	Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos);
	Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, unsigned int texture);
	~Coin();

	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

