#pragma once
#include "LightStructs.h"
#include "DebugLightMesh.h"
#include "Camera.h"

const std::vector<Vertex> vertices23 = {
{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  0.0f}},
	{{0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},

	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},

	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},

	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},

	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},

	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}}
};


template <class T>
class Light : VulkanResources
{
private:
	UniformBuffer LightBuffer;

	void CreateUniformBuffers(VulkanRenderer& Renderer)
	{
		LightBuffer = UniformBuffer(Renderer, sizeof(T));
		DebugMesh = DebugLightMesh(Renderer, vertices23);
	}

	void UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
	{
		void* UniformData;
		vkMapMemory(*GetDevice(Renderer), UniformBufferMemory, 0, UniformSize, 0, &UniformData);
		memcpy(UniformData, UniformObjectData, UniformSize);
		vkUnmapMemory(*GetDevice(Renderer), UniformBufferMemory);

		DebugMesh = DebugLightMesh(Renderer, vertices23);
	}

protected:
	T LightSettings;

public:

	DebugLightMesh DebugMesh;
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

	void UpdateDebugMesh(VulkanRenderer& Renderer, PositionMatrix ubo2)
	{
		DebugMesh.UpdateUniformBuffer(Renderer, ubo2);
	}

	void DrawDebugMesh(VulkanRenderer& Renderer, uint32_t DrawFrame)
	{
		DebugMesh.Draw(Renderer, DrawFrame);
	}

	void Destroy(VulkanRenderer& Renderer)
	{
		DebugMesh.Destroy(Renderer);
		LightBuffer.Destroy(Renderer);
	}
};


