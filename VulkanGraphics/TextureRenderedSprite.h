#pragma once
#include "Sprite.h"
#include "TextureRenderPass.h"

class TextureRenderedSprite : public Sprite
{
private:
	TextureRenderPass TextureRenderer;

public:

	TextureRenderedSprite();
	TextureRenderedSprite(RendererManager& renderer);
	TextureRenderedSprite(RendererManager& renderer, int renderBitFlags);

	void UpdateSwapChain(RendererManager& renderer);

	virtual void RendererUpdate(RendererManager& renderer);
	virtual void Destory(RendererManager& renderer) override;

	std::shared_ptr<Texture> GetRenderedTextureImage() { return TextureRenderer.GetRenderedTextureImage(); }
	VkDescriptorSet GetIMGuiImage() { return TextureRenderer.GetIMGuiImage(); }
};

