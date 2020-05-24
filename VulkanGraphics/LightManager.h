#pragma once
#include "Camera.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"

class LightManager : VulkanResources
{
private:

public:
	LightManager();
	~LightManager();

	std::vector<DirectionalLight> DirectionalLightList;
	std::vector<PointLight> PointLightList;
	std::vector<SpotLight> SpotlightList;

	void Update(VulkanRenderer& Renderer, Camera camera);
	void UpdateLights();
	void DrawDebugMesh(VulkanRenderer& Renderer, uint32_t DrawFrame);
	void Destroy(VulkanRenderer& Renderer);
};

