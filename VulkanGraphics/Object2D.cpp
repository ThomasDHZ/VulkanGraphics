#include "Object2D.h"

Object2D::Object2D()
{
}

Object2D::~Object2D()
{
}

void Object2D::DrawMessage(RendererManager& renderer)
{
	ObjectMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.collisionDebugPipeline);
	if (RenderBitFlags & RenderBitFlag::RenderOnMainPass)
	{
		ObjectMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.renderer2DPipeline);
	}
	if (RenderBitFlags & RenderBitFlag::RenderOnTexturePass)
	{
		ObjectMesh->CreateDrawMessage(renderer, 2, renderer.textureRenderer.renderer2DPipeline);
	}
	if (RenderBitFlags & RenderBitFlag::RenderMainPass)
	{
		ObjectMesh->CreateDrawMessage(renderer, 4, renderer.sceneRenderer.renderer2DPipeline);
	}
	if (RenderBitFlags & RenderBitFlag::RenderEffectPass)
	{
		ObjectMesh->CreateDrawMessage(renderer, 5, renderer.forwardRenderer.renderer2DPipeline);
	}
	if (RenderBitFlags & RenderBitFlag::RenderShadow)
	{
		ObjectMesh->CreateDrawMessage(renderer, 7, renderer.shadowRenderer.renderer2DPipeline);
	}
}

void Object2D::AnimationHandler()
{
}

void Object2D::Update(RendererManager& renderer, float dt, std::shared_ptr<Camera> camera, LightBufferObject Lightbuffer)
{
	AnimationHandler();
	ObjectMesh->Update(renderer, camera, Lightbuffer);
	for (auto& collider : ObjectColliderList)
	{
		collider->Update(ObjectMesh->GetPosition3D());
	}
}

void Object2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
}

void Object2D::Destory(RendererManager& renderer)
{
	ObjectMesh->Destory(renderer);
}

void Object2D::SetPosition2D(glm::vec2 Pos)
{
	ObjectMesh->SetPosition3D(glm::vec3(Pos, ObjectMesh->GetPosition3D().z));
}

void Object2D::SetPosition2D(float x, float y)
{
	ObjectMesh->SetPosition3D(glm::vec3(x, y, ObjectMesh->GetPosition3D().z));
}

void Object2D::SetPosition3D(glm::vec3 Pos)
{
	ObjectMesh->SetPosition3D(Pos);
}

void Object2D::SetPosition3D(float x, float y, float z)
{
	ObjectMesh->SetPosition3D(glm::vec3(x, y, z));
}