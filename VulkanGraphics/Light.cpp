#include "Light.h"

Light::Light()
{
}

Light::Light(RendererManager& renderer, VkDescriptorSetLayout& descriptorSetLayout, int renderBit, glm::vec3 Pos)
{
	light.pLight.position = glm::vec3(1.0f, 1.0f, 0.0f);
	light.pLight.ambient = glm::vec3(0.8f, 0.8f, 0.86f);
	light.dLight.direction = glm::vec3(0.5f, 1.0f, 0.3f);

	LightMesh = std::make_unique<DebugLightMesh>(DebugLightMesh(renderer, LightVertices, LightIndices, descriptorSetLayout, renderBit));
	renderer.AddDrawableMesh(LightMesh);
}

Light::~Light()
{
}

void Light::Update(RendererManager& renderer, OrthographicCamera& camera)
{
	light.viewPos = camera.GetPosition();

	MeshColor color = {};
	color.Color = glm::vec3(1.0f, 0.0f, 0.0f);

	LightMesh->MeshPosition = light.pLight.position;
	LightMesh->Update(renderer, camera, color);
}
