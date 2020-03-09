#pragma once
#include <vector>
#include "Mesh.h"
#include "ModelLoader.h"

class Model
{
private:
	VulkanDevice DeviceInfo;
	std::vector<Mesh> MeshList;

public:
	Model();
	Model(MainPipeline pipeline, VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList);
	~Model();

	void UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, MainPipeline pipeline, int currentImage);
	void RecreateSwapChainStage(MainPipeline pipeline);
	void Destory();
};

