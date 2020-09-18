#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures layout)
{
	skyBoxMesh = std::make_shared<SkyBoxMesh>(SkyBoxMesh(renderer, textureManager, layout));
}

SkyBox::~SkyBox()
{
}

void SkyBox::Update(RendererManager& renderer, std::shared_ptr<Camera> camera)
{
	skyBoxMesh->UpdateUniformBuffer(renderer, camera);
}
