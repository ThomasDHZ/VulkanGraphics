#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
private:
	void CreateTextureSampler(VulkanRenderer& renderer);

public:

	Texture2D();
	Texture2D(VulkanRenderer& renderer, std::string TexturePath);
	~Texture2D();
};

