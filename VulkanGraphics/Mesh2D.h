#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <map>
#include "RendererColorTexture.h"
#include "RendererManager.h"
#include "NewMesh.h"

class Mesh2D : public NewMesh
{
private:
    void CreateDescriptorPool(RendererManager& renderer) override;
    void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager) override;

public:

    Mesh2D();
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures);
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer);
    ~Mesh2D();
};