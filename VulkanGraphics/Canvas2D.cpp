#include "Canvas2D.h"

Canvas2D::Canvas2D()
{
}

Canvas2D::Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize)
{
	DeviceInfo = deviceInfo;
	ClearColor = clearColor;
	CanvasSize = canvasSize;


		CanvasTexture[0] = Texture2D(DeviceInfo, canvasSize.x, canvasSize.y, clearColor);
		CanvasTexture[1] = Texture2D(DeviceInfo, canvasSize.x, canvasSize.y, clearColor);
		CanvasTexture[2] = Texture2D(DeviceInfo, canvasSize.x, canvasSize.y, clearColor);

	std::vector<Texture2D> textureList = { CanvasTexture[0], CanvasTexture[1], CanvasTexture[2] };
	CanvasMesh = Screen2DMesh(deviceInfo, textureList);
}

Canvas2D::Canvas2D(VulkanDevice deviceInfo, Pixel clearColor, glm::ivec2 canvasSize, Texture2D backgroundTexture)
{
	DeviceInfo = deviceInfo;
	ClearColor = clearColor;
	CanvasSize = canvasSize;

	CanvasTexture[0] = backgroundTexture;
	CanvasTexture[1] = backgroundTexture;
	CanvasTexture[2] = backgroundTexture;

	std::vector<Texture2D> textureList = { CanvasTexture[0], CanvasTexture[1], CanvasTexture[2] };

	CanvasMesh = Screen2DMesh(deviceInfo, textureList);
}

void Canvas2D::SetPixel(glm::ivec2 position, Pixel pixel)
{
	for (auto frame : CanvasTexture)
	{
		frame.SetPixel(position, pixel);
	}
}

void Canvas2D::UpdateFrame(size_t currentFrame)
{
	//CanvasTexture[currentFrame].SetPixel(glm::ivec2(rand() % CanvasSize.x, rand() % CanvasSize.y), Pixel(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF));
	CanvasMesh.ClearSwapChain();
	CanvasMesh.UpdateSwapChain(CanvasTexture[currentFrame]);
}

void Canvas2D::UpdateSwapChain(size_t currentFrame)
{
	CanvasMesh.UpdateSwapChain(CanvasTexture[currentFrame]);
}

void Canvas2D::Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage)
{
	CanvasMesh.Draw(commandbuffer, ShaderPipeline, ShaderPipelineLayout, currentImage);
}

void Canvas2D::ClearSwapChain()
{
	CanvasMesh.ClearSwapChain();
}

void Canvas2D::Destory()
{
	for (auto frame : CanvasTexture)
	{
		frame.Destroy();
	}
	CanvasMesh.Destory();
}
