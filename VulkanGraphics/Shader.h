#pragma once
#include "BaseShader.h"
struct ShaderTextureInputs
{
	VkImageView SkyboxtextureImageView;
	VkSampler SkyboxtextureSampler;
	VkImageView textureImageView;
	VkSampler textureSampler;
	VkImageView textureImageView2;
	VkSampler textureSampler2;
};

class Shader : public BaseShader
{
private:
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkDescriptorSetLayout layout, ShaderTextureInputs TextureInfo);
public:
	Shader();
	Shader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout layout, ShaderTextureInputs ShaderInput);
	~Shader();

	void RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout layout,  ShaderTextureInputs ShaderInput);
};

