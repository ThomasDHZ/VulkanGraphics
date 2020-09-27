#pragma once

#include "FrameBufferMesh.h"
class FrameBufferRenderPass
{
private:
	FramebufferRenderer FrameBufferRenderer;
	FrameBufferMesh FrameBuffer;

	void RenderPass(RendererManager& renderer);

public:
	FrameBufferRenderPass();
	FrameBufferRenderPass(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> TextureImage, std::shared_ptr<Texture> BloomImage);
	~FrameBufferRenderPass();

	void Update(RendererManager& renderer);
	void Draw(RendererManager& renderer);
	void UpdateSwapChain(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> TextureImage, std::shared_ptr<Texture> BloomImage);
	void Destory(RendererManager& renderer);
};

