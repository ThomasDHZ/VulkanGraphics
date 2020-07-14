#pragma once
#include "Texture.h"

class EngineRenderedTexture : public Texture
{
private:
	void CreateEngineRendereredImageHolder(Renderer& renderer, VkFormat ImageFormat);
	void CreateEngineRendereredImageView(Renderer& renderer, VkFormat ImageFormat);
	void CreateEngineRendereredImageSampler(Renderer& renderer);
public:
	EngineRenderedTexture();
	EngineRenderedTexture(Renderer& renderer, VkFormat ImageFormat, VkExtent2D TextureSize);
	~EngineRenderedTexture();
};

