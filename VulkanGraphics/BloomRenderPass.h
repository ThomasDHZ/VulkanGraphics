#pragma once
#include "RendererManager.h"
#include "FrameBufferTextureRenderer.h"
#include "FrameBufferMesh.h"

class BloomRenderPass
{
	private:
		FrameBufferTextureRenderer BloomRenderer;
		FrameBufferTextureRenderer BloomRenderer2;
		FrameBufferMesh BloomFrameBuffer;
		FrameBufferMesh BloomFrameBuffer2;

		void RenderBloomPass(RendererManager& renderer);
		void RenderBloomPass2(RendererManager& renderer);

	public:
		BloomRenderPass();
		BloomRenderPass(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage);
		~BloomRenderPass();

		void Draw(RendererManager& renderer);
		void UpdateSwapChain(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage);
		void Destory(RendererManager& renderer);

		std::shared_ptr<Texture> OutputBloomImage() { return BloomRenderer2.ColorTexture; }
};

