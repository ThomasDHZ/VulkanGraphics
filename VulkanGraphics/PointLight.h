#pragma once
#include "Light.h"

class PointLight : public Light<PointLightBuffer>
{
public:

	PointLight();
	PointLight(Renderer& renderer, PointLightBuffer settings);
	~PointLight();

	void SetPosition(float Position[2]);
	void SetPosition(glm::vec3 Position);
	void SetAmbient(float Position[2]);
	void SetAmbient(glm::vec3 Ambient);
	void SetDiffuse(float Position[2]);
	void SetDiffuse(glm::vec3 Diffuse);
	void SetSpecular(float Position[2]);
	void SetSpecular(glm::vec3 Specular);
	void SetConstant(float Constant);
	void SetLinear(float Linear);
	void SetQuadratic(float Quadratic);

	glm::vec3 GetPosition();
	glm::vec3 GetDiffuse();
	glm::vec3 GetAmbient();
	glm::vec3 GetSpecular();
	float GetConstant();
	float GetLinear();
	float GetQuadratic();

	PointLightBuffer GetSettings() { return LightSettings; }
	glm::vec3* GetLightPos() { return &LightSettings.Position; }
};
