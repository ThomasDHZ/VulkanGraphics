#include "Light.h"

Light::Light()
{
}

Light::Light(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos)
{
	light.dLight.direction = glm::vec3(0.5f, 1.0f, 0.3f);

	for(int x= 0; x < 4; x++)
	{
		light.pLight[x].position = glm::vec3(1.0f, 10.0f, 1.2f);
		light.pLight[x].ambient = glm::vec3(0.8f, 0.8f, 0.86f);
		LightMeshList.emplace_back(std::make_shared<DebugLightMesh>(DebugLightMesh(renderer, textureManager, renderBit)));
	}
}

Light::~Light()
{
}

void Light::Update(RendererManager& renderer, std::shared_ptr<Camera>& camera)
{
	light.viewPos = static_cast<OrthographicCamera*>(camera.get())->GetPosition();

	MeshColor color = {};
	color.Color = glm::vec3(1.0f, 0.0f, 0.0f);


	for (int x = 0; x < 4; x++)
	{
		LightMeshList[x]->MeshPosition = light.pLight[x].position;
		LightMeshList[x]->Update(renderer, camera, color);
	}
}

void Light::Destory(RendererManager& renderer)
{

	for (int x = 0; x < 4; x++)
	{
		LightMeshList[x]->Destory(renderer);
	}
}
