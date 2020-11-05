#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::LightManager(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos)
{
	light.dLight.direction = glm::vec3(0.5f, 1.0f, 0.3f);

	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(renderer, textureManager, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(renderer, textureManager, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(renderer, textureManager, renderBit, Pos)));
	PointLightList.emplace_back(std::make_shared<PointLight>(PointLight(renderer, textureManager, renderBit, Pos)));
}

LightManager::~LightManager()
{
}

void LightManager::Update(RendererManager& renderer, std::shared_ptr<Camera>& camera)
{
	light.viewPos = static_cast<OrthographicCamera*>(camera.get())->GetPosition();

	for (int x = 0; x < PointLightList.size(); x++)
	{
		PointLightList[x]->Update(renderer, camera);
		light.pLight[x] = PointLightList[x]->pointLight;
	}
}

void LightManager::Destory(RendererManager& renderer)
{
	for (auto pointLight : PointLightList)
	{
		pointLight->Destory(renderer);
	}
}
