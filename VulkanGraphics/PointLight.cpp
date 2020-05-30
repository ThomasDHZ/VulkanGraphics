#include "PointLight.h"

PointLight::PointLight() : Light<PointLightBuffer>()
{
}

PointLight::PointLight(Renderer& renderer, PointLightBuffer settings) : Light<PointLightBuffer>(renderer, settings)
{
}

PointLight::~PointLight()
{
}

void PointLight::SetPosition(float Position[2])
{
	LightSettings.Position.x = Position[0];
	LightSettings.Position.y = Position[1];
	LightSettings.Position.z = Position[2];
}

void PointLight::SetPosition(glm::vec3 Position)
{
	LightSettings.Position = Position;
}

void PointLight::SetAmbient(float Position[2])
{
	LightSettings.Ambient.x = Position[0];
	LightSettings.Ambient.y = Position[1];
	LightSettings.Ambient.z = Position[2];
}

void PointLight::SetAmbient(glm::vec3 Ambient)
{
	LightSettings.Ambient = Ambient;
}

void PointLight::SetDiffuse(float Position[2])
{
	LightSettings.Diffuse.x = Position[0];
	LightSettings.Diffuse.y = Position[1];
	LightSettings.Diffuse.z = Position[2];
}

void PointLight::SetDiffuse(glm::vec3 Diffuse)
{
	LightSettings.Diffuse = Diffuse;
}

void PointLight::SetSpecular(float Position[2])
{
	LightSettings.Specular.x = Position[0];
	LightSettings.Specular.y = Position[1];
	LightSettings.Specular.z = Position[2];
}

void PointLight::SetSpecular(glm::vec3 Specular)
{
	LightSettings.Specular = Specular;
}

void PointLight::SetConstant(float Constant)
{
	LightSettings.Constant = Constant;
}

void PointLight::SetLinear(float Linear)
{
	LightSettings.Linear = Linear;
}

void PointLight::SetQuadratic(float Quadratic)
{
	LightSettings.Quadratic = Quadratic;
}

glm::vec3 PointLight::GetPosition()
{
	return LightSettings.Position;
}

glm::vec3 PointLight::GetDiffuse()
{
	return LightSettings.Diffuse;
}

glm::vec3 PointLight::GetAmbient()
{
	return LightSettings.Ambient;
}

glm::vec3 PointLight::GetSpecular()
{
	return LightSettings.Specular;
}

float PointLight::GetConstant()
{
	return LightSettings.Constant;
}

float PointLight::GetLinear()
{
	return LightSettings.Linear;
}

float PointLight::GetQuadratic()
{
	return LightSettings.Quadratic;
}
