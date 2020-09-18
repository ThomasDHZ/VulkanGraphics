#pragma once
#include "Mesh2D.h"
#include "DebugLightMesh.h"
#include "RendererManager.h"

const std::vector<Vertex> LightVertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
};

const std::vector<uint16_t> LightIndices = {
	0, 1, 2, 2, 3, 0
};

class Light
{
private:


public:
	std::shared_ptr<DebugLightMesh> LightMesh;
	LightBufferObject light;

	Light();
	Light(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos);
	~Light();

	void Update(RendererManager& renderer, std::shared_ptr<Camera>& camera);
	void Destory(RendererManager& renderer);
};

