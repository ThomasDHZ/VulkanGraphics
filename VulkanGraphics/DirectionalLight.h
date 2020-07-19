//#pragma once
//#include "Light.h"
//
//class DirectionalLight : public Light<DirectionalLightBuffer>
//{
//public:
//
//	DirectionalLight();
//	DirectionalLight(Renderer& renderer, DirectionalLightBuffer settings);
//	~DirectionalLight();
//
//	void SetAmbient(glm::vec3 Ambient);
//	void SetDiffuse(glm::vec3 Diffuse);
//	void SetDirection(glm::vec3 Direction);
//	void SetSpecular(glm::vec3 Specular);
//
//	glm::vec3 GetAmbient();
//	glm::vec3 GetDiffuse();
//	glm::vec3 GetDirection();
//	glm::vec3 GetSpecular();
//
//	DirectionalLightBuffer GetSettings() { return LightSettings; }
//};