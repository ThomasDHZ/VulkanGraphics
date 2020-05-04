#pragma once
#include "Light.h"

struct AmbientLightUniformBuffer
{
	alignas(16) glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	alignas(4) float Strength = 1.0f;
};

class AmbientLight : public Light
{
private:
	AmbientLightUniformBuffer LightSettings;
	std::vector<VkBuffer>  AmbientLightUniformBuffers;
	std::vector<VkDeviceMemory>  AmbientLightUniformBuffersMemory;

	void CreateUniformBuffers(VulkanRenderer& Renderer);
public:
	AmbientLight();
	AmbientLight(VulkanRenderer& Renderer, AmbientLightUniformBuffer lightSettings);
	AmbientLight(VulkanRenderer& Renderer, float LightStrength, glm::vec3 LightColor);
	~AmbientLight();

	void UpdateUniformBuffer(VulkanRenderer& Renderer, AmbientLightUniformBuffer AmbientLightBuffer, int currentImage);
	AmbientLightUniformBuffer GetLightSettings() { return LightSettings; }
	glm::vec3* GetColorPtr() { return &LightSettings.Color; }
	float* GetColorStrengthPtr() { return &LightSettings.Strength; }
};

