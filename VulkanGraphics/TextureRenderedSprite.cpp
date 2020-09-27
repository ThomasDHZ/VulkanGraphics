#include "TextureRenderedSprite.h"

TextureRenderedSprite::TextureRenderedSprite() : Sprite()
{
}

TextureRenderedSprite::TextureRenderedSprite(RendererManager& renderer)
{
	TextureRenderer = TextureRenderPass(renderer);
}

TextureRenderedSprite::TextureRenderedSprite(RendererManager& renderer, int renderBitFlags) : Sprite(renderer, renderBitFlags)
{
	TextureRenderer = TextureRenderPass(renderer);
}

void TextureRenderedSprite::UpdateSwapChain(RendererManager& renderer)
{
	TextureRenderer.UpdateSwapChain(renderer);
}

void TextureRenderedSprite::RendererUpdate(RendererManager& renderer)
{
	TextureRenderer.Draw(renderer);
}

void TextureRenderedSprite::Destory(RendererManager& renderer)
{
	TextureRenderer.Destory(renderer);
	Sprite::Destory(renderer);
}
