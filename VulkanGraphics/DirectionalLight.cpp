#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light<DirectionalLightBuffer>()
{
}

DirectionalLight::DirectionalLight(VulkanRenderer& Renderer, DirectionalLightBuffer settings) : Light<DirectionalLightBuffer>(Renderer, settings)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetAmbient(glm::vec3 Ambient)
{
	LightSettings.Ambient = Ambient;
}
void DirectionalLight::SetDiffuse(glm::vec3 Diffuse)
{
	LightSettings.Diffuse = Diffuse;
}
void DirectionalLight::SetDirection(glm::vec3 Direction)
{
	LightSettings.Direction = Direction;
}
void DirectionalLight::SetSpecular(glm::vec3 Specular)
{
	LightSettings.Specular = Specular;
}

glm::vec3 DirectionalLight::GetAmbient() { return LightSettings.Ambient; }
glm::vec3 DirectionalLight::GetDiffuse() { return LightSettings.Diffuse; }
glm::vec3 DirectionalLight::GetDirection() { return LightSettings.Direction; }
glm::vec3 DirectionalLight::GetSpecular() { return LightSettings.Specular; }