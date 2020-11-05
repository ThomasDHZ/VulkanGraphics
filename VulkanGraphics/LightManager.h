#pragma once
#include "PointLight.h"

class LightManager
{
private:

public:
	std::vector<std::shared_ptr<PointLight>> PointLightList;
	LightBufferObject light;

	LightManager();
	LightManager(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos);
	~LightManager();

	void Update(RendererManager& renderer, std::shared_ptr<Camera>& camera);
	void Destory(RendererManager& renderer);
};

