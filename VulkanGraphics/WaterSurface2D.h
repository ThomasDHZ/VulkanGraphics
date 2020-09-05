#pragma once
#include "Sprite.h"
class WaterSurface2D : public Sprite
{
private:
public:
	WaterSurface2D();
	WaterSurface2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize);
	WaterSurface2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize, Texture& texture);
	~WaterSurface2D();

	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

