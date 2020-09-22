#pragma once
#include "Sprite.h"

class MMShot : public Sprite
{
private:
	static constexpr glm::vec2 SpriteSize = glm::vec2(0.15f, 0.15f);
	Animation2D StandAni;
	Animation2D RunAni;
public:
	MMShot();
	MMShot(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos);
	~MMShot();

	void Update(RendererManager& renderer, float dt, std::shared_ptr<Camera> camera, LightBufferObject light) override;
	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList) override;
};

