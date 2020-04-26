#include "Canvas2D.h"

Canvas2D::Canvas2D()
{
}

Canvas2D::Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize)
{
	DeviceInfo = deviceInfo;
	ClearColor = clearColor;
	CanvasSize = canvasSize;

	CanvasTexture = Texture2D(DeviceInfo, canvasSize.x, canvasSize.y, clearColor);

	std::vector<Texture2D> textureList = { CanvasTexture };
	CanvasMesh = Screen2DMesh(deviceInfo, textureList);
}

Canvas2D::Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize, Texture2D backgroundTexture, Texture2D SpriteLayer)
{
	DeviceInfo = deviceInfo;
	ClearColor = clearColor;
	CanvasSize = canvasSize;

	background = backgroundTexture;
	spriteLayer = SpriteLayer;
	copySpriteLayer = SpriteLayer;

	CanvasTexture = Texture2D(DeviceInfo, canvasSize.x, canvasSize.y, clearColor);

	DQ1Sprite = Sprite(DeviceInfo, glm::ivec2(0,0));

	std::vector<Texture2D> textureList = { CanvasTexture, spriteLayer };
	CanvasMesh = Screen2DMesh(deviceInfo, textureList);
}

void Canvas2D::SetPixel(glm::ivec2 position, Pixel pixel)
{
	CanvasTexture.SetPixel(position, pixel);
}

void Canvas2D::UpdateFrame(int MapX, int MapY, int SpriteX, int SpriteY)
{
	//CanvasMesh.ClearSwapChain();
	spriteLayer = copySpriteLayer;
	spriteLayer.CopyRange(DQ1Sprite.GetSpriteTexture(), SpriteX, SpriteY);
	CanvasTexture.CopyRange(background, MapX, MapY);
	CanvasMesh.UpdateSwapChain(CanvasTexture, spriteLayer);
}

void Canvas2D::UpdateSwapChain()
{
	CanvasMesh.UpdateSwapChain(CanvasTexture, spriteLayer);
}

void Canvas2D::Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage)
{
	CanvasMesh.Draw(commandbuffer, ShaderPipeline, ShaderPipelineLayout, currentImage);
}

void Canvas2D::ClearSwapChain()
{
	//CanvasMesh.ClearSwapChain();
}

void Canvas2D::Destory()
{
	CanvasTexture.Destroy();
	spriteLayer.Destroy();
	CanvasMesh.Destory();
}
