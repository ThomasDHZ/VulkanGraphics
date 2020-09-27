#include "FrameBufferRenderPass.h"

FrameBufferRenderPass::FrameBufferRenderPass()
{
}

FrameBufferRenderPass::FrameBufferRenderPass(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> TextureImage, std::shared_ptr<Texture> BloomImage)
{
	FrameBufferRenderer = FramebufferRenderer(renderer);
	FrameBuffer = FrameBufferMesh(renderer, textureManager, TextureImage, BloomImage);
}

FrameBufferRenderPass::~FrameBufferRenderPass()
{
}

void FrameBufferRenderPass::Update(RendererManager& renderer)
{
	FrameBufferRenderer.UpdateSwapChain(renderer);
}

void FrameBufferRenderPass::Draw(RendererManager& renderer)
{
	RenderPass(renderer);
}

void FrameBufferRenderPass::UpdateSwapChain(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> TextureImage, std::shared_ptr<Texture> BloomImage)
{
	FrameBufferRenderer.UpdateSwapChain(renderer);
	FrameBuffer.ScreenResizeUpdate(renderer, textureManager, TextureImage, BloomImage);
}

void FrameBufferRenderPass::Destory(RendererManager& renderer)
{
	FrameBufferRenderer.Destroy(renderer);
	FrameBuffer.Destory(renderer);
}

void FrameBufferRenderPass::RenderPass(RendererManager& renderer)
{
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderer.forwardRenderer.RenderPass;
	renderPassInfo.framebuffer = renderer.forwardRenderer.SwapChainFramebuffers[renderer.DrawFrame];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	for (auto drawMessage : renderer.DrawMessageList)
	{
		if (drawMessage->RendererID == 0)
		{
			FrameBufferRenderer.Draw(renderer, drawMessage);
		}
	}
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.RenderCommandBuffer[renderer.DrawFrame]);
	vkCmdEndRenderPass(renderer.RenderCommandBuffer[renderer.DrawFrame]);
}