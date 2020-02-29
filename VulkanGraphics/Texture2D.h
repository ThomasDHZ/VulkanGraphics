#pragma once
#include <vulkan\vulkan_core.h>
#include "Texture.h"
#include "Model.h"

class Texture2D : public Texture
{
private:
	void CreateTextureImage(std::string TexturePath);
	void CreateTextureSampler();

public:

	Texture2D();
	Texture2D(VulkanDevice deviceInfo, std::string TexturePath);
	~Texture2D();
};

