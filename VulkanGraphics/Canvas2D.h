#pragma once
#include "Texture2D.h"
#include "Screen2DMesh.h"

class Canvas2D
{
private:
	VulkanDevice DeviceInfo;
	Screen2DMesh CanvasMesh;
	Texture2D CanvasTexture;
	Texture2D background;
	Texture2D spriteLayer;
	Texture2D copySpriteLayer;
	Texture2D CatSprite;
	Pixel ClearColor;
	glm::ivec2 CanvasSize;

public:
	Canvas2D();
	Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize);
	Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize, Texture2D backgroundTexture, Texture2D SpriteLayer);

	void SetPixel(glm::ivec2 position, Pixel pixel);
	void UpdateFrame(int MapX, int MapY, int SpriteX, int SpriteY);
	void UpdateSwapChain();
	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void ClearSwapChain();
	void Destory();
};

