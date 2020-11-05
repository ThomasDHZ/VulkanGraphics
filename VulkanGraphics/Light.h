#pragma once
#include "Mesh2D.h"
#include "DebugLightMesh.h"
#include "RendererManager.h"


class Light
{
private:

public:
	Light();
	Light(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, int renderBit, glm::vec3 Pos);
	~Light();
};

