#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& descriptorSetLayout, CubeMapLayout layout)
{
	textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), layout);
	skyBoxMesh = std::make_shared<SkyBoxMesh>(SkyBoxMesh(renderer, textureManager, descriptorSetLayout, 0));
	renderer.AddDrawableMesh(skyBoxMesh);
}

SkyBox::~SkyBox()
{
}
