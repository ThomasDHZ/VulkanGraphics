#pragma once
#include "Light.h"

class SpotLight : public Light<SpotLightBuffer>
{
public:

	SpotLight();
	SpotLight(Renderer& renderer, SpotLightBuffer settings);
	~SpotLight();

	void SetPosition(glm::vec3 Position);
	void SetDirection(glm::vec3 Direction);
	void SetAmbient(glm::vec3 Ambient);
	void SetDiffuse(glm::vec3 Diffuse);
	void SetSpecular(glm::vec3 Specular);
	void SetCutOff(float CutOff);
	void SetOuterCutOff(float OuterCutOff);
	void SetConstant(float Constant);
	void SetLinear(float Linear);
	void SetQuadratic(float Quadratic);

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	glm::vec3 GetAmbient();
	glm::vec3 GetDiffuse();
	glm::vec3 GetSpecular();
	float GetCutOff();
	float GetOuterCutOff();
	float GetConstant();
	float GetLinear();
	float GetQuadratic();

	SpotLightBuffer GetSettings() { return LightSettings; }
};