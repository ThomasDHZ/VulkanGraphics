#pragma once
#include "VulkanResources.h"
#include "UniformBuffer.h"

struct DirectionalLightBuffer
{
	alignas(16) glm::vec3 Direction;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
};

struct PointLightBuffer
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;

	alignas(4) float Constant;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
};

struct SpotLightBuffer
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Direction;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;

	alignas(4) float CutOff;
	alignas(4) float OuterCutOff;
	alignas(4) float Constant;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
};

template <class T>
class Light :  VulkanResources
{
private:
	UniformBuffer LightBuffer;

	void CreateUniformBuffers(VulkanRenderer& Renderer)
	{
		LightBuffer = UniformBuffer(Renderer, sizeof(T));
	}

	void UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
	{
		void* UniformData;
		vkMapMemory(*GetDevice(Renderer), UniformBufferMemory, 0, UniformSize, 0, &UniformData);
		memcpy(UniformData, UniformObjectData, UniformSize);
		vkUnmapMemory(*GetDevice(Renderer), UniformBufferMemory);
	}

protected:
	T LightSettings;

public:
	Light()
	{
	}

	Light(VulkanRenderer& Renderer, T settings)
	{
		LightSettings = settings;
		CreateUniformBuffers(Renderer);
	}

	~Light()
	{
	}

	void UpdateUniformBuffer(VulkanRenderer& Renderer, PointLightBuffer PointLightInfo, int currentImage)
	{
		LightBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&PointLightInfo), Renderer.DrawFrame);
	}

	void Destroy(VulkanRenderer& Renderer)
	{
		LightBuffer.Destroy(Renderer);
	}
};


