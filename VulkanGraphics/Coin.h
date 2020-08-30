#pragma once
#include "Sprite.h"
class Coin : public Sprite
{
private:
	static constexpr glm::vec2 SpriteSize = glm::vec2(1.0f, 1.0f);
public:
	Coin();
	Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos);
	~Coin();

	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

