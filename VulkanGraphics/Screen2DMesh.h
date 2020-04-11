#pragma once
#include "BaseMesh.h"

const std::vector<Vertex2D> ScreenVertices =
{
	{{-1.0f,  1.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f}, {0.0f, 0.0f}},
	{{ 1.0f, -1.0f}, {1.0f, 0.0f}},

	{{-1.0f,  1.0f}, {0.0f, 1.0f}},
	{{ 1.0f, -1.0f}, {1.0f, 0.0f}},
	{{ 1.0f,  1.0f}, {1.0f, 1.0f}}
};

class Screen2DMesh : public BaseMesh
{
private:

	void CreateVertexBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets(const Texture2D& CurrentScreenTexture, const Texture2D& SpriteLayer);

public:
	Screen2DMesh();
	Screen2DMesh(VulkanDevice deviceInfo, std::vector<Texture2D>& TextureList);
	~Screen2DMesh();

	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void UpdateSwapChain(const Texture2D& CurrentScreenTexture, const Texture2D& SpriteScreenTexture);
};


