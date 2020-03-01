#pragma once
#include "BaseShader.h"
#include "CubeMapTexture.h"
#include "Skybox.h"

class SkyBoxShader : public BaseShader
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);

public:
	SkyBoxShader();
	SkyBoxShader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, CubeMapTexture cubeMapTexture);
	~SkyBoxShader();

	void RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, CubeMapTexture cubeMapTexture);
};

