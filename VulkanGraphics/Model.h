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
	Model( VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList);
	~Model();

	void UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage();
	void Destory();
};

