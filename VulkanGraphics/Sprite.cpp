#include "Sprite.h"



Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos)
{

	const std::vector<uint16_t> SpriteIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, SpriteVertices, SpriteIndices, SpriteTextures));

	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);

	DrawMessage(renderer);
}

void Sprite::SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, CustomBuffer custom)
{
	const std::vector<uint16_t> SpriteIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, SpriteVertices, SpriteIndices, SpriteTextures, custom));

	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);

	DrawMessage(renderer);
}

void Sprite::DrawMessage(RendererManager& renderer)
{
	if (RenderBitFlags & RenderBitFlag::RenderOnMainPass)
	{
		SpriteMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.renderer2DPipeline);
	}
	if (RenderBitFlags & RenderBitFlag::RenderOnTexturePass)
	{
		SpriteMesh->CreateDrawMessage(renderer, 2, renderer.textureRenderer.renderer2DPipeline);
	}
}

bool Sprite::OnGroundCheck(std::vector<BoxCollider> SpriteList)
{
	for (auto& sprite : SpriteList)
	{
		if (collider.CollidesWith(sprite, Gravity))
		{
			return true;
		}
	}
	return false;
}

void Sprite::ApplyGravity(std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	if (ObjectFlagBits & ObjectFlags::ApplyGravity)
	{
		Move(SpriteList, Gravity);
	}
}

void Sprite::ApplyGravity(std::vector<BoxCollider> SpriteList)
{
	if (ObjectFlagBits & ObjectFlags::ApplyGravity)
	{
		Move(SpriteList, Gravity);
	}
}

void Sprite::Move(std::vector<std::shared_ptr<Sprite>> SpriteList, glm::vec3 MoveDirection)
{
	for (auto& sprite : SpriteList)
	{
		if (sprite.get() != this &&
			sprite->ObjectFlagBits & ObjectFlags::Wall)
		{
			if (collider.CollidesWith(sprite->collider, MoveDirection))
			{
				MoveDirection = glm::vec3(0.0f);
				break;
			}
		}
	}

	SpriteMesh->MeshPosition += MoveDirection;
}

void Sprite::Move(std::vector<BoxCollider> SpriteList, glm::vec3 MoveDirection)
{
	for (auto& sprite : SpriteList)
	{
		if (collider.CollidesWith(sprite, MoveDirection))
		{
			MoveDirection = glm::vec3(0.0f);
			break;
		}
	}
	SpriteMesh->MeshPosition += MoveDirection;
}

void Sprite::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	CurrentAni.Update();
	SpriteMesh->properites.UVOffset = CurrentAni.GetCurrentFrame().GetUVOffset();
	SpriteMesh->properites.UVScale = CurrentAni.GetCurrentFrame().GetUVScale();

	SpriteMesh->Update(renderer, camera, light);
	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->GetPosition3D() + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);

	AnimationHandler();
}

void Sprite::AnimationHandler()
{
	int a = 34;
}


//void Sprite::Draw(VulkanRenderer& renderer, int currentFrame)
//{
//	///SpriteMesh.Draw(renderer, currentFrame);
//}

void Sprite::Destory(RendererManager& renderer)
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);
	SpriteMesh->Destory(renderer);
}

void Sprite::SetPosition2D(glm::vec2 Pos)
{
	SpriteMesh->SetPosition3D(glm::vec3(Pos, SpriteMesh->GetPosition3D().z));
}

void Sprite::SetPosition2D(float x, float y)
{
	SpriteMesh->SetPosition3D(glm::vec3(x, y, SpriteMesh->GetPosition3D().z));
}

void Sprite::SetPosition3D(glm::vec3 Pos)
{
	SpriteMesh->SetPosition3D(Pos);
}

void Sprite::SetPosition3D(float x, float y, float z)
{
	SpriteMesh->SetPosition3D(glm::vec3(x, y, z));
}