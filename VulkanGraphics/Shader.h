#pragma once
#include "BaseShader.h"

class Shader : public BaseShader
{
private:

	void CreateDescriptorSetLayout();
public:
	Shader();
	Shader(VulkanDevice deviceInfo);
	~Shader();

	void CreateShaderPipeLine();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkImageView SkyboxtextureImageView, VkSampler SkyboxtextureSampler, VkImageView textureImageView, VkSampler textureSampler, VkImageView textureImageView2, VkSampler textureSampler2);
};

