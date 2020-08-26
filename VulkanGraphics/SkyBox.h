#pragma once
#include "SkyBoxMesh.h"
#include "RendererManager.h"
class SkyBox
{
private:
	std::shared_ptr<SkyBoxMesh> skyBoxMesh;
public:
	SkyBox();
	SkyBox(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, CubeMapLayout layout);
	~SkyBox();
};

