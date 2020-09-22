#pragma once
#include "TextureRenderer.h"
#include "Mesh2D.h"
class BloomRenderer : public RendererBase
{
private:
	Mesh2D mesh;
	TextureRenderer textureRenderer;
	std::shared_ptr<RendererDrawMessage> RenderMessage;

public:
	BloomRenderer();
	BloomRenderer(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<GraphicsPipeline> Shader);
	~BloomRenderer();

	void Render(RendererManager& renderer);
	std::shared_ptr<RendererColorTexture> GetColorTexture() { return textureRenderer.ColorTexture; };
};

