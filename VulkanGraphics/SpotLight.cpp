#include "SpotLight.h"

SpotLight::SpotLight() : Light<SpotLightBuffer>()
{
}

SpotLight::SpotLight(VulkanRenderer& Renderer, SpotLightBuffer settings) : Light<SpotLightBuffer>(Renderer, settings)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetPosition(glm::vec3 Position)
{
	LightSettings.Position = Position;
}

void SpotLight::SetDirection(glm::vec3 Direction)
{
	LightSettings.Direction = Direction;
}

void SpotLight::SetAmbient(glm::vec3 Ambient)
{
	LightSettings.Ambient = Ambient;
}

void SpotLight::SetDiffuse(glm::vec3 Diffuse)
{
	LightSettings.Diffuse = Diffuse;
}

void SpotLight::SetSpecular(glm::vec3 Specular)
{
	LightSettings.Specular = Specular;
}

void SpotLight::SetCutOff(float CutOff)
{
	LightSettings.CutOff = CutOff;
}

void SpotLight::SetOuterCutOff(float OuterCutOff)
{
	LightSettings.OuterCutOff = OuterCutOff;
}

void SpotLight::SetConstant(float Constant)
{
	LightSettings.Constant = Constant;
}

void SpotLight::SetLinear(float Linear)
{
	LightSettings.Linear = Linear;
}

void SpotLight::SetQuadratic(float Quadratic)
{
	LightSettings.Quadratic = Quadratic;
}

glm::vec3 SpotLight::GetPosition()
{
	return LightSettings.Position;
}

glm::vec3 SpotLight::GetDirection()
{
	return LightSettings.Direction;
}

glm::vec3 SpotLight::GetAmbient()
{
	return LightSettings.Ambient;
}

glm::vec3 SpotLight::GetDiffuse()
{
	return LightSettings.Diffuse;
}

glm::vec3 SpotLight::GetSpecular()
{
	return LightSettings.Specular;
}

float SpotLight::GetCutOff()
{
	return LightSettings.CutOff;
}

float SpotLight::GetOuterCutOff()
{
	return LightSettings.OuterCutOff;
}

float SpotLight::GetConstant()
{
	return LightSettings.Constant;
}

float SpotLight::GetLinear()
{
	return LightSettings.Linear;
}

float SpotLight::GetQuadratic()
{
	return LightSettings.Quadratic;
}
