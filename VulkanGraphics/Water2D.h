#pragma once
#include "Sprite.h"
#include "WaterSurface2D.h"

class Water2D : public Sprite
{
private:
	OrthographicCamera WaterCamera;
	void DrawMessage(RendererManager& renderer) override;

public:
	Water2D();
	Water2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos, glm::vec2 WaterSize, const OrthographicCamera& camera, const std::shared_ptr<Texture> texture);
	~Water2D();

	WaveProperites waveprop;
	void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light) override;
	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) override;
};