//#pragma once
//#include <vector>
//#include "Mesh.h"
//#include "ModelLoader.h"
//
//class Model
//{
//private:
//	VulkanDevice DeviceInfo;
//	std::vector<Mesh> MeshList;
//
//public:
//	Model();
//	Model( VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList);
//	~Model();
//
//	std::string ModelName;
//	glm::vec3 ModelPosition = glm::vec3();
//	glm::vec3 ModelRotate = glm::vec3(0.0f, 0.0f, 1.0f);
//	glm::vec3 ModelScale = glm::vec3(1.0f);
//
//	void UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage);
//	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
//	void SecBufferDraw(VkCommandBuffer& commandbuffer, VkCommandBufferBeginInfo cmdInfo, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentFrame);
//	void UpdateSwapChain();
//	void Destory();
//
//	std::vector<Mesh> GetModelMeshList()
//	{
//		return MeshList;
//	}
//};
//
