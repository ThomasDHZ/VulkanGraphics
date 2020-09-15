#pragma once
#include "SkyBoxMesh.h"
#include "RendererManager.h"
class SkyBox
{
private:

public:
	SkyBox();
	SkyBox(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, MeshTextures layout);
	~SkyBox();

	std::shared_ptr<SkyBoxMesh> skyBoxMesh;
	void Update(RendererManager& renderer, OrthographicCamera& camera);
};

