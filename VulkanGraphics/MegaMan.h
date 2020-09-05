#pragma once
#include "Sprite.h"
#include "MMShot.h"

class MegaMan : public Sprite
{
private:
	static constexpr glm::vec2 SpriteSize = glm::vec2(1.0f, 1.0f);
	Animation2D StandAni;
	Animation2D RunAni;

public:
	MegaMan();
	MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos);
	~MegaMan();

	void Update(GLFWwindow* window, RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light, std::vector<std::shared_ptr<Sprite>> SpriteList, std::vector<BoxCollider> LevelCollidorList, std::shared_ptr<TextureManager>textureManager);
	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};

