#include "BloomRenderPass.h"

BloomRenderPass::BloomRenderPass()
{
}

BloomRenderPass::BloomRenderPass(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage)
{
	BloomRenderer = FrameBufferTextureRenderer(renderer);
	BloomRenderer2 = FrameBufferTextureRenderer(renderer);

	BloomFrameBuffer = FrameBufferMesh(renderer,  textureManager, FrameBufferImage, FrameBufferImage, 5, renderer.EffectRenderer.bloomPipeline);
	BloomFrameBuffer2 = FrameBufferMesh(renderer, textureManager, renderer.EffectRenderer.ColorTexture, renderer.EffectRenderer.ColorTexture, 6, renderer.EffectRenderer.bloomPipeline2nd);
}

BloomRenderPass::~BloomRenderPass()
{
}

void BloomRenderPass::Draw(RendererManager& renderer)
{
	RenderBloomPass(renderer);
	RenderBloomPass2(renderer);
}

void BloomRenderPass::UpdateSwapChain(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage)
{
	BloomRenderer.UpdateSwapChain(renderer);
	BloomRenderer2.UpdateSwapChain(renderer);

	BloomFrameBuffer.ScreenResizeUpdate(renderer, textureManager, FrameBufferImage, FrameBufferImage, 5, renderer.EffectRenderer.bloomPipeline);
	BloomFrameBuffer2.ScreenResizeUpdate(renderer, textureManager, renderer.EffectRenderer.ColorTexture, renderer.EffectRenderer.ColorTexture, 6, renderer.EffectRenderer.bloomPipeline2nd);
}

void BloomRenderPass::Destory(RendererManager& renderer)
{
	BloomRenderer.Destroy(renderer);
	BloomRenderer2.Destroy(renderer);

	BloomFrameBuffer.Destory(renderer);
	BloomFrameBuffer2.Destory(renderer);
}

void BloomRenderPass::RenderBloomPass(RendererManager& renderer)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = BloomRenderer.RenderPass;
	renderPassInfo.framebuffer = BloomRenderer.SwapChainFramebuffers[renderer.DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	for (auto drawMessage : renderer.DrawMessageList)
	{
		if (drawMessage->RendererID == 5)
		{
			BloomRenderer.Draw(renderer, drawMessage);
		}
	}
	vkCmdEndRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame]);
}

void BloomRenderPass::RenderBloomPass2(RendererManager& renderer)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = BloomRenderer2.RenderPass;
	renderPassInfo.framebuffer = BloomRenderer2.SwapChainFramebuffers[renderer.DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	for (auto drawMessage : renderer.DrawMessageList)
	{
		if (drawMessage->RendererID == 6)
		{
			BloomRenderer2.Draw(renderer, drawMessage);
		}
	}
	vkCmdEndRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame]);
}
