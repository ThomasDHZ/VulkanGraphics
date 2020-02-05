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
	void CreateDescriptorSets(SkyBox skybox, VkImageView textureImageView, VkSampler textureSampler, VkImageView textureImageView2, VkSampler textureSampler2);
};

