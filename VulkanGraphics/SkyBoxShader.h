#pragma once
#include "BaseShader.h"
#include "CubeMapTexture.h"
#include "Skybox.h"

class SkyBoxShader : public BaseShader
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(CubeMapTexture cubeMapTexture);

public:
	SkyBoxShader();
	SkyBoxShader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, CubeMapTexture cubeMapTexture);
	~SkyBoxShader();

	void RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, CubeMapTexture cubeMapTexture);
};

