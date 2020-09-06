#include "MegaMan.h"

MegaMan::MegaMan() : Sprite()
{
}

MegaMan::MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.14f / 3, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.14f / 3, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

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
	MegaManTextures.SpecularMap = "texture/MegaMan_Specular.bmp";
	MegaManTextures.NormalMap = "texture/MegaMan_Normal.bmp";
	MegaManTextures.AlphaMap = "texture/MegaMan_Alpha.bmp";

	ObjectFlagBits = ObjectFlags::Player | ObjectFlags::ApplyGravity;
	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass;

	SetUpSprite(renderer, textureManager, MegaManVertices, MegaManTextures, StartPos);
}

MegaMan::~MegaMan()
{
}

void MegaMan::Update(GLFWwindow* window, RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light, std::vector<std::shared_ptr<Sprite>> SpriteList, std::vector<BoxCollider> LevelCollidorList, std::shared_ptr<TextureManager>textureManager)
{
	glm::vec3 MoveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			MoveDirection = glm::vec3(-0.01f, 0.0f, 0.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			MoveDirection = glm::vec3(0.01f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			MoveDirection = glm::vec3(0.0f, -0.01f, 0.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			MoveDirection = glm::vec3(0.0f, 0.01f, 0.0f);
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		auto shot = std::make_shared<MMShot>(MMShot(renderer, textureManager, glm::vec2(SpriteMesh->MeshPosition.x + 1.0f, SpriteMesh->MeshPosition.y + 0.5f)));
		SpriteList.emplace_back(shot);
	}

	Move(LevelCollidorList, MoveDirection);
	Sprite::Update(renderer, camera, light);
}

void MegaMan::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	int a = 34;
}
