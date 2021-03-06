#pragma once
#include "Mesh.h"
#include "GameManager.h"

class Object
{
private:
public:
	std::shared_ptr<Mesh> ObjectMesh;

	Object();
	Object(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
	~Object();
};

