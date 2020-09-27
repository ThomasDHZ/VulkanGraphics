#include "TextureRenderPass.h"

TextureRenderPass::TextureRenderPass()
{
}

TextureRenderPass::TextureRenderPass(RendererManager& renderer)
{
	textureRenderer = TextureRenderer(renderer);
}

TextureRenderPass::~TextureRenderPass()
{
}

void TextureRenderPass::Draw(RendererManager& renderer)
{
	RenderTexturePass(renderer);
}

void TextureRenderPass::UpdateSwapChain(RendererManager& renderer)
{
	textureRenderer.UpdateSwapChain(renderer);
}

void TextureRenderPass::Destory(RendererManager& renderer)
{
	textureRenderer.Destroy(renderer);
}

void TextureRenderPass::RenderTexturePass(RendererManager& renderer)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = textureRenderer.RenderPass;
	renderPassInfo.framebuffer = textureRenderer.SwapChainFramebuffers[renderer.DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	for (auto drawMessage : renderer.DrawMessageList)
	{
		if (drawMessage->RendererID == 2)
		{
			textureRenderer.Draw(renderer, drawMessage);
		}
	}
	vkCmdEndRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame]);
}

