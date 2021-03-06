#pragma once
#include "Sprite.h"
#include "TextureRenderer.h"
class EnergyTank : public Sprite
{
private:

	static constexpr glm::vec2 SpriteSize = glm::vec2(0.5f, 0.5f);
	Animation2D EnergyTankFlicker;
public:
	EnergyTank();
	EnergyTank(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos);
	~EnergyTank();

	void Update(RendererManager& renderer, float dt, std::shared_ptr<Camera> camera, LightBufferObject light) override;
	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList) override;
};

