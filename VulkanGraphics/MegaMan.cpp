#include "MegaMan.h"

MegaMan::MegaMan() : Sprite()
{
}

MegaMan::MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos) : Sprite()
{
	AnimationFrame[Stand1] = glm::vec2(0.011f, 0.0f);
	AnimationFrame[Stand2] = glm::vec2(0.144f, 0.0f);
	AnimationFrame[StartRun] = glm::vec2(0.279f, 0.0f);
	AnimationFrame[Run1] = glm::vec2(0.422f, 0.0f);
	AnimationFrame[Run2] = glm::vec2(0.568f, 0.0f);
	AnimationFrame[Run3] = glm::vec2(0.700f, 0.0f);
	AnimationFrame[Run4] = glm::vec2(0.822f, 0.0f);
	
	std::vector<glm::vec2> StandFrames = { AnimationFrame[Stand1] , AnimationFrame[Stand2] };
	std::vector<glm::vec2> RunFrames = { AnimationFrame[Run1] , AnimationFrame[Run2], AnimationFrame[Run3], AnimationFrame[Run4] };
	
	StandAni = Animation2D(StandFrames, 1.0f, 0);
	RunAni = Animation2D(RunFrames, 0.1f, 1);
	CurrentAni = StandAni;

	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "texture/MegaMan_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "texture/container2_specular.png";
	MegaManTextures.NormalMap = "texture/SparkMan_normal.bmp";
	MegaManTextures.AlphaMap = "texture/MegaManAlpha.bmp";

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.14f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.14f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = SpriteType::SMegaMan;
	ObjectFlagBits = ObjectFlags::Player | ObjectFlags::ApplyGravity;
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

void MegaMan::Update(GLFWwindow* window, RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x)
			{*/
			//SpriteMesh->MeshPosition.x -= 0.001f;
			//SpriteMesh->properites.FlipTexture = 1;
			//camera.Position.x -= 0.001f;
			/*}*/
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x - 1.0f)
			{*/
			//SpriteMesh->MeshPosition.x += 0.001f;
			//SpriteMesh->properites.FlipTexture = 0;
			//camera.Position.x += 0.001f;
			//}
			//else
			//{
			//	SpriteMesh.MeshPosition.x = otherSprite.MeshPosition.x - 1.0f;
			//}
		}
		if (CurrentAni.GetAnimationID() != RunAni.GetAnimationID())
		{
			CurrentAni = RunAni;
		}
	}
	else
	{
		if (CurrentAni.GetAnimationID() != StandAni.GetAnimationID())
		{
			CurrentAni = StandAni;
		}
	}
	CurrentAni.Update();
	Sprite::Update(renderer, camera, light);
}

void MegaMan::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	int a = 34;
}
