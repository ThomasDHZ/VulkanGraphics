#pragma once
#include "BaseShader.h"

class SkyBoxShader : public BaseShader
{
private:

	void CreateDescriptorSetLayout();
public:
	SkyBoxShader();
	SkyBoxShader(VulkanDevice deviceInfo);
	~SkyBoxShader();

	void CreateShaderPipeLine();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkImageView textureImageView, VkSampler textureSampler);
};

