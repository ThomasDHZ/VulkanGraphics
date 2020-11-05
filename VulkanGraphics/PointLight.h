#pragma once
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

class PointLight
{
private:
	std::shared_ptr<DebugLightMesh> DebugMesh;
public:
	PointLightStruct pointLight;

	PointLight();
	PointLight(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 position);
	~PointLight();

	void Update(RendererManager& renderer, std::shared_ptr<Camera>& camera);
	void Destory(RendererManager& renderer);
};

