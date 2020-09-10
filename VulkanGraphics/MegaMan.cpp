#include "MegaMan.h"
#include "Water2D.h"

MegaMan::MegaMan() : Sprite()
{
}

MegaMan::MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, -0.1f},				 {0.0f, 0.0f, 1.0f}, {0.0521f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, -0.1f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.0521f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	std::vector<Frame2D> StandFrames;
	StandFrames.emplace_back(Frame2D(glm::vec2(0.0f, 0.0f)));
	StandFrames.emplace_back(Frame2D(glm::vec2(0.0521f, 0.0f)));
	StandFrames.emplace_back(Frame2D(glm::vec2(0.1021f, 0.0f)));

	std::vector<Frame2D> RunFrames;
	RunFrames.emplace_back(Frame2D(glm::vec2(0.16459f, 0.0f)));
	RunFrames.emplace_back(Frame2D(glm::vec2(0.2167f, 0.0f)));
	RunFrames.emplace_back(Frame2D(glm::vec2(0.2708f, 0.0f)));
	RunFrames.emplace_back(Frame2D(glm::vec2(0.2167f, 0.0f)));

	std::vector<Frame2D> SlideFrames;
	SlideFrames.emplace_back(Frame2D(glm::vec2(0.3333f, 0.0f), glm::vec2(1.6875f, 1.0f), 3.0f));

	std::vector<Frame2D> JumpFrames;
	JumpFrames.emplace_back(Frame2D(glm::vec2(0.40412f, 0.0f)));

	std::vector<Frame2D> ShootFrames;
	ShootFrames.emplace_back(Frame2D(glm::vec2(0.60625f, 0.0f)));

	StandAni = Animation2D(StandFrames, 2.0f, 0);
	RunAni = Animation2D(RunFrames, 0.1f, 1);
	SlideAni = Animation2D(SlideFrames, 6.1f, 2);
	JumpAni = Animation2D(JumpFrames, 6.1, 3);
	ShootAni = Animation2D(ShootFrames, 6.1, 4);

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

void MegaMan::Update(GLFWwindow* window, RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light, std::vector<std::shared_ptr<Sprite>> SpriteList, std::vector<std::shared_ptr<ColliderObject>> TileColliderList, std::shared_ptr<TextureManager>textureManager)
{
	if (OnGroundCheck(TileColliderList))
	{
		MegaManStateBitFlag |= MegaManStatesFlag::StateOnGround;
	}
	else
	{
		MegaManStateBitFlag &= ~MegaManStatesFlag::StateOnGround;
	}

	glm::vec3 MoveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			SpriteMesh->properites.ReflectSprite = true;
			MoveDirection = glm::vec3(-0.01f, 0.0f, 0.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			SpriteMesh->properites.ReflectSprite = false;
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

		MegaManStateBitFlag |= MegaManStatesFlag::StateRunning;
	}
	else
	{
		MegaManStateBitFlag &= ~MegaManStatesFlag::StateRunning;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		CurrentAni = ShootAni;
		auto shot = std::make_shared<MMShot>(MMShot(renderer, textureManager, glm::vec2(SpriteMesh->GetPosition2D().x + 1.0f, SpriteMesh->GetPosition2D().y + 0.5f)));
		SpriteList.emplace_back(shot);
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
	//	MegaManStateBitFlag |= MegaManStatesFlag::StateJump;
		SpriteMesh->MeshPosition.y += 0.1f;
	}

	Move(TileColliderList, MoveDirection);
	Sprite::Update(renderer, camera, light);
}

void MegaMan::AnimationHandler()
{
	if (MegaManStateBitFlag & MegaManStatesFlag::StateOnGround)
	{
		if (CurrentAni.GetAnimationID() != StandAni.GetAnimationID())
		{
			CurrentAni = StandAni;
		}
	}
	else
	{
		CurrentAni = JumpAni;
	}

	if (MegaManStateBitFlag & MegaManStatesFlag::StateRunning | MegaManStatesFlag::StateOnGround)
	{
		if (CurrentAni.GetAnimationID() != StandAni.GetAnimationID())
		{
			CurrentAni = RunAni;
		}
	}
}

void MegaMan::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	for (auto& sprite : SpriteList)
	{
		if (dynamic_cast<Water2D*>(sprite.get()))
		{
			if (sprite->collider.CollidesWith(collider))
			{
				MegaManStateBitFlag |= MegaManStatesFlag::StateInWater;
				break;
			}
			else
			{
				MegaManStateBitFlag &= ~MegaManStatesFlag::StateInWater;
			}
		}
	}
}
