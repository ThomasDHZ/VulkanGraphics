//#include "MegaMan.h"
//
//MegaMan::MegaMan() : Sprite()
//{
//}
//
//MegaMan::MegaMan(Renderer& renderer, TextureMaps SpriteMaps, glm::vec2 StartPos) : Sprite(renderer, 1.0f, 1.0f, SpriteMaps, StartPos, SpriteType::SMegaMan)
//{
//	AnimationFrame[Stand1] = glm::vec2(0.011f, 0.0f);
//	AnimationFrame[Stand2] = glm::vec2(0.144f, 0.0f);
//	AnimationFrame[StartRun] = glm::vec2(0.279f, 0.0f);
//	AnimationFrame[Run1] = glm::vec2(0.422f, 0.0f);
//	AnimationFrame[Run2] = glm::vec2(0.568f, 0.0f);
//	AnimationFrame[Run3] = glm::vec2(0.700f, 0.0f);
//	AnimationFrame[Run4] = glm::vec2(0.822f, 0.0f);
//
//	std::vector<glm::vec2> StandFrames = { AnimationFrame[Stand1] , AnimationFrame[Stand2] };
//	std::vector<glm::vec2> RunFrames = { AnimationFrame[Run1] , AnimationFrame[Run2], AnimationFrame[Run3], AnimationFrame[Run4] };
//
//	StandAni = Animation2D(StandFrames, 1.0f, 0);
//	RunAni = Animation2D(RunFrames, 0.1f, 1);
//	CurrentAni = StandAni;
//}
//
//MegaMan::MegaMan(Renderer& renderer, TextureMaps SpriteMaps, glm::vec3 StartPos) : Sprite(renderer, 1.0f, 1.0f, SpriteMaps, StartPos, SpriteType::SMegaMan)
//{
//
//
//	AnimationFrame[Stand1] = glm::vec2(0.011f, 0.0f);
//	AnimationFrame[Stand2] = glm::vec2(0.144f, 0.0f);
//	AnimationFrame[StartRun] = glm::vec2(0.279f, 0.0f);
//	AnimationFrame[Run1] = glm::vec2(0.422f, 0.0f);
//	AnimationFrame[Run2] = glm::vec2(0.568f, 0.0f);
//	AnimationFrame[Run3] = glm::vec2(0.700f, 0.0f);
//	AnimationFrame[Run4] = glm::vec2(0.822f, 0.0f);
//
//	std::vector<glm::vec2> StandFrames = { AnimationFrame[Stand1] , AnimationFrame[Stand2] };
//	std::vector<glm::vec2> RunFrames = { AnimationFrame[Run1] , AnimationFrame[Run2], AnimationFrame[Run3], AnimationFrame[Run4] };
//
//	StandAni = Animation2D(StandFrames, 1.0f, 0);
//	RunAni = Animation2D(RunFrames, 0.1f, 1);
//	CurrentAni = StandAni;
//}
//
//MegaMan::~MegaMan()
//{
//}
//
//void MegaMan::Update(GLFWwindow* window, Renderer& renderer, Camera& camera, Lights light)
//{
//	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
//		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//	{
//		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//		{
//			/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x)
//			{*/
//			SpriteMesh.MeshPosition.x -= 0.001f;
//			SpriteMesh.properites.FlipTexture = 1;
//			camera.Position.x -= 0.001f;
//			/*}*/
//		}
//		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//		{
//			/*if (SpriteMesh.MeshPosition.x < otherSprite.MeshPosition.x - 1.0f)
//			{*/
//			SpriteMesh.MeshPosition.x += 0.001f;
//			SpriteMesh.properites.FlipTexture = 0;
//			camera.Position.x += 0.001f;
//			//}
//			//else
//			//{
//			//	SpriteMesh.MeshPosition.x = otherSprite.MeshPosition.x - 1.0f;
//			//}
//		}
//		if (CurrentAni.GetAnimationID() != RunAni.GetAnimationID())
//		{
//			CurrentAni = RunAni;
//		}
//	}
//	else
//	{
//		if (CurrentAni.GetAnimationID() != StandAni.GetAnimationID())
//		{
//			CurrentAni = StandAni;
//		}
//	}
//
//	Sprite::Update(renderer, camera, light);
//}
