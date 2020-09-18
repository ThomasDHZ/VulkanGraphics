#include "Light.h"

Light::Light()
{
}

Light::Light(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos)
{
	light.pLight.position = glm::vec3(1.0f, 10.0f, 1.2f);
	light.pLight.ambient = glm::vec3(0.8f, 0.8f, 0.86f);
	light.dLight.direction = glm::vec3(0.5f, 1.0f, 0.3f);

	LightMesh = std::make_shared<DebugLightMesh>(DebugLightMesh(renderer, textureManager, renderBit));
}

Light::~Light()
{
}

void Light::Update(RendererManager& renderer, std::shared_ptr<Camera>& camera)
{
	light.viewPos = static_cast<OrthographicCamera*>(camera.get())->GetPosition();

	MeshColor color = {};
	color.Color = glm::vec3(1.0f, 0.0f, 0.0f);

	LightMesh->MeshPosition = light.pLight.position;
	LightMesh->Update(renderer, camera, color);
}

void Light::Destory(RendererManager& renderer)
{
	LightMesh->Destory(renderer);
}
