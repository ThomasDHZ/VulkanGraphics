#include "MegaMan.h"

MegaMan::MegaMan() : Sprite()
{
}

MegaMan::MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos) : Sprite()
{
	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "texture/MegaMan_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "texture/container2_specular.png";
	MegaManTextures.NormalMap = "texture/SparkMan_normal.bmp";
	MegaManTextures.AlphaMap = "texture/MegaManAlpha.bmp";

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.4f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.4f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = SpriteType::SMegaMan;
	SpriteMaps = MegaManTextures;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, MegaManVertices, MegaManIndices, MegaManTextures, descriptorSetLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	renderer.AddDrawableMesh(SpriteMesh);
	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

MegaMan::~MegaMan()
{
}

void MegaMan::Update(GLFWwindow* window, VulkanRenderer& renderer, Camera& camera, UniformBufferObject light)
{
	//if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	//{
	//	if (!collider.CollidesWith(SpriteList[1]->collider, glm::vec3(-0.01f, 0.0f, 0.0f)))
	//	{
	//		SpriteList[0]->SpriteMesh->MeshPosition.x -= 0.01f;
	//	}
	//}
	//else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//{
	//	if (!collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.01f, 0.0f, 0.0f)))
	//	{
	//		SpriteList[0]->SpriteMesh->MeshPosition.x += 0.01f;
	//	}

	//}
	//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	//{
	//	if (!collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.0f, -0.01f, 0.0f)))
	//	{
	//		SpriteList[0]->SpriteMesh->MeshPosition.y -= 0.01f;
	//	}
	//}
	//else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	//{
	//	if (!collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.0f, 0.01f, 0.0f)))
	//	{
	//		SpriteList[0]->SpriteMesh->MeshPosition.y += 0.01f;
	//	}
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
	//	glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//{
	//	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	//	{
	//		/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x)
	//		{*/
	//		SpriteMesh.MeshPosition.x -= 0.001f;
	//		SpriteMesh.properites.FlipTexture = 1;
	//		camera.Position.x -= 0.001f;
	//		/*}*/
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//	{
	//		/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x - 1.0f)
	//		{*/
	//		SpriteMesh.MeshPosition.x += 0.001f;
	//		SpriteMesh.properites.FlipTexture = 0;
	//		camera.Position.x += 0.001f;
	//		//}
	//		//else
	//		//{
	//		//	SpriteMesh.MeshPosition.x = otherSprite.MeshPosition.x - 1.0f;
	//		//}
	//	}
	//	if (CurrentAni.GetAnimationID() != RunAni.GetAnimationID())
	//	{
	//		CurrentAni = RunAni;
	//	}
	//}
	//else
	//{
	//	if (CurrentAni.GetAnimationID() != StandAni.GetAnimationID())
	//	{
	//		CurrentAni = StandAni;
	//	}
	//}

	//Sprite::Update(renderer, camera, light);
}

void MegaMan::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	int a = 34;
}
