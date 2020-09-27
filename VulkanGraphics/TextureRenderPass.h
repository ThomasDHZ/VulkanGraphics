#pragma once
#include "TextureRenderer.h"
#include "RendererManager.h"

class TextureRenderPass
{
private:
	TextureRenderer textureRenderer;

	void RenderTexturePass(RendererManager& renderer);

public:
	TextureRenderPass();
	TextureRenderPass(RendererManager& renderer);
	~TextureRenderPass();

	void Draw(RendererManager& renderer);
	void UpdateSwapChain(RendererManager& renderer);
	void Destory(RendererManager& renderer);

	std::shared_ptr<Texture> GetRenderedTextureImage() { return textureRenderer.ColorTexture; }
	VkDescriptorSet GetIMGuiImage() { return textureRenderer.ColorTexture->ImGuiDescriptorSet; }
};