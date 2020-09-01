#pragma once
#include "Sprite.h"

class MegaMan : public Sprite
{
private:
	static constexpr glm::vec2 SpriteSize = glm::vec2(1.0f, 1.0f);
	Animation2D StandAni;
	Animation2D RunAni;
public:
	MegaMan();
	MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos);
	~MegaMan();

	void Update(GLFWwindow* window, RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light);
	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

