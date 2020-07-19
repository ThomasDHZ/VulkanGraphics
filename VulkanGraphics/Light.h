//#pragma once
//#include "LightStructs.h"
//#include "DebugLightMesh.h"
//#include "Camera.h"
//
//const std::vector<Vertex> vertices23 = {
//{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  0.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},
//
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
//	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
//
//	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f,  0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//
//	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
//
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}}
//};
//
//
//template <class T>
//class Light : VulkanResources
//{
//private:
//	UniformBuffer LightBuffer;
//
//	void CreateUniformBuffers(Renderer& renderer)
//	{
//		LightBuffer = UniformBuffer(renderer, sizeof(T));
//		DebugMesh = DebugLightMesh(renderer, vertices23);
//	}
//
//	void UpdateUniformBuffer(Renderer& renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
//	{
//		void* UniformData;
//		vkMapMemory(*GetDevice(renderer), UniformBufferMemory, 0, UniformSize, 0, &UniformData);
//		memcpy(UniformData, UniformObjectData, UniformSize);
//		vkUnmapMemory(*GetDevice(renderer), UniformBufferMemory);
//
//		DebugMesh = DebugLightMesh(renderer, vertices23);
//	}
//
//protected:
//
//
//public:
//	T LightSettings;
//	DebugLightMesh DebugMesh;
//	Light()
//	{
//	}
//
//	Light(Renderer& renderer, T settings)
//	{
//		LightSettings = settings;
//		CreateUniformBuffers(renderer);
//	}
//
//	~Light()
//	{
//	}
//
//	void UpdateUniformBuffer(Renderer& renderer, PointLightBuffer PointLightInfo, int currentImage)
//	{
//		LightBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&PointLightInfo), renderer.DrawFrame);
//	}
//
//	void UpdateDebugMesh(Renderer& renderer, PositionMatrix ubo2)
//	{
//		DebugMesh.UpdateUniformBuffer(renderer, ubo2);
//	}
//
//	void DrawDebugMesh(Renderer& renderer, uint32_t DrawFrame)
//	{
//		DebugMesh.Draw(renderer, DrawFrame);
//	}
//
//	void Destroy(Renderer& renderer)
//	{
//		DebugMesh.Destroy(renderer);
//		LightBuffer.Destroy(renderer);
//	}
//};
//
//
