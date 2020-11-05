#include "PointLight.h"

PointLight::PointLight()
{
}

PointLight::PointLight(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 position)
{
	pointLight.position = glm::vec3(1.0f, 10.0f, 1.2f);
	pointLight.ambient = glm::vec3(0.8f, 0.8f, 0.86f);
	pointLight.InUseFlag = 1;
	DebugMesh = std::make_shared<DebugLightMesh>(DebugLightMesh(renderer, textureManager, renderBit));

}

PointLight::~PointLight()
{
}

void PointLight::Update(RendererManager& renderer, std::shared_ptr<Camera>& camera)
{
	MeshColor color = {};
	color.Color = pointLight.diffuse;

	DebugMesh->MeshPosition = pointLight.position;
	DebugMesh->Update(renderer, camera, color);
}

void PointLight::Destory(RendererManager& renderer)
{
	DebugMesh->Destory(renderer);
}
