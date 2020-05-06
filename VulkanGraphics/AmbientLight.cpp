#include "AmbientLight.h"
#include "VulkanBufferManager.h"

AmbientLight::AmbientLight() : Light()
{
}

AmbientLight::AmbientLight(VulkanRenderer& Renderer, AmbientLightUniformBuffer lightSettings) : Light(Renderer)
{
	LightSettings = lightSettings;

	CreateUniformBuffers(Renderer);
}

AmbientLight::AmbientLight(VulkanRenderer& Renderer, float LightStrength, glm::vec3 LightColor) : Light(Renderer)
{
	LightSettings.Strength = LightStrength;
	LightSettings.Color = LightColor;

	CreateUniformBuffers(Renderer);
}

AmbientLight::~AmbientLight()
{
}

void AmbientLight::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(AmbientLight);

	AmbientLightUniformBuffers.resize(GetSwapChainImageCount(Renderer));
	AmbientLightUniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, AmbientLightUniformBuffers[i], AmbientLightUniformBuffersMemory[i]);
	}
}

void AmbientLight::UpdateUniformBuffer(VulkanRenderer& Renderer, AmbientLightUniformBuffer AmbientLightBuffer, int currentImage)
{
	Light::UpdateUniformBuffer(Renderer, AmbientLightUniformBuffersMemory[currentImage], static_cast<void*>(&AmbientLightBuffer), sizeof(AmbientLightBuffer));
}

void AmbientLight::Destroy(VulkanRenderer& Renderer)
{
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		vkDestroyBuffer(*GetDevice(Renderer), AmbientLightUniformBuffers[i], nullptr);
		vkFreeMemory(*GetDevice(Renderer), AmbientLightUniformBuffersMemory[i], nullptr);

		AmbientLightUniformBuffers[i] = VK_NULL_HANDLE;
		AmbientLightUniformBuffersMemory[i] = VK_NULL_HANDLE;
	}
}
