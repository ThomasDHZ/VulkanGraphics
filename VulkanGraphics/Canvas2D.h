#pragma once
#include "Texture2D.h"
#include "Screen2DMesh.h"

class Canvas2D
{
private:
	VulkanDevice DeviceInfo;
	Screen2DMesh CanvasMesh;
	std::array<Texture2D, 3> CanvasTexture;
	Pixel ClearColor;
	glm::ivec2 CanvasSize;

public:
	Canvas2D();
	Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize);
	Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize, Texture2D backgroundTexture);

	void SetPixel(glm::ivec2 position, Pixel pixel);
	void UpdateFrame(size_t currentFrame);
	void UpdateSwapChain(size_t currentFrame);
	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void ClearSwapChain();
	void Destory();
};

