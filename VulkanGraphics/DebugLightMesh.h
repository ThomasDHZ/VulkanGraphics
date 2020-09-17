#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"
#include "Camera.h"
#include "Mesh.h"

const std::vector<Vertex> CubeVertices =
{
	{{-0.5,-0.5,-0.5}, {0,-1,0}, {0,1}},
	{{0.5,-0.5,-0.5}, {0,-1,0}, {1,1}},
	{{0.5,-0.5,0.5}, {0,-1,0}, {1,0}},
	{{-0.5,-0.5,0.5}, {0,-1,0}, {0,0}},
	{{-0.5,0.5,-0.5}, {0,0,-1}, {0,0}},
	{{0.5,0.5,-0.5}, {0,0,-1}, {1,0}},
	{{0.5,-0.5,-0.5}, {0,0,-1}, {1,1}},
	{{-0.5,-0.5,-0.5}, {0,0,-1}, {0,1}},
	{{0.5,0.5,-0.5}, {1,0,0}, {0,0}},
	{{0.5,0.5,0.5}, {1,0,0}, {1,0}},
	{{0.5,-0.5,0.5}, {1,0,0}, {1,1}},
	{{0.5,-0.5,-0.5}, {1,0,0}, {0,1}},
	{{0.5,0.5,0.5}, {0,0,1}, {1,0}},
	{{-0.5,0.5,0.5}, {0,0,1}, {0,0}},
	{{-0.5,-0.5,0.5}, {0,0,1}, {0,1}},
	{{0.5,-0.5,0.5}, {0,0,1}, {1,1}},
	{{-0.5,0.5,0.5}, {-1,0,0}, {1,0}},
	{{-0.5,0.5,-0.5}, {-1,0,0}, {0,0}},
	{{-0.5,-0.5,-0.5}, {-1,0,0}, {0,1}},
	{{-0.5,-0.5,0.5}, {-1,0,0}, {1,1}},
	{{-0.5,0.5,-0.5}, {0,1,0}, {0,1}},
	{{-0.5,0.5,0.5}, {0,1,0}, {0,0}},
	{{0.5,0.5,0.5}, {0,1,0}, {1,0}},
	{{0.5,0.5,-0.5}, {0,1,0}, {1,1}},
};

const std::vector<uint16_t> CubeIndices = {
	0,1,2, 0,2,3,
	4,5,6, 4,6,7,
	8,9,10, 8,10,11,
	12,13,14, 12,14,15,
	16,17,18, 16,18,19,
	20,21,22, 20,22,23,
};

struct MeshColor
{
    alignas(16) glm::vec3 Color;
};

class DebugLightMesh : public Mesh
{
private:
    VulkanUniformBuffer meshColorBuffer;

    virtual void CreateUniformBuffers(RendererManager& renderer) override;
    virtual void CreateDescriptorPool(RendererManager& renderer) override;
    virtual void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager) override;
    virtual void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, MeshColor meshColorBuffer);

public:

    DebugLightMesh();
    DebugLightMesh(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, int renderBit);
    ~DebugLightMesh();

    void Update(RendererManager& renderer, Camera& camera, MeshColor meshColorBuffer);
    void Update(RendererManager& renderer, OrthographicCamera& camera, MeshColor meshColorBuffer);
    virtual void Destory(RendererManager& renderer) override;
};

