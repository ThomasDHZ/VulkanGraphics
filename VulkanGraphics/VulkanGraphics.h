#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "VulkanResources.h"
#include "GUIDebugger.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Skybox.h"
#include "AmbientLight.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


const std::vector<Vertex> vertices =
{
	{{-0.5,-0.5,-0.5}, {0,-1,0}, {0,1}},
	{{0.5,-0.5,-0.5}, {0,-1,0}, {1,1}},
	{{0.5,-0.5,0.5}, {0,-1,0}, {1,0}},
	{{-0.5,-0.5,0.5}, {0,-1,0}, {0,0}},
	{{-0.5,0.5,-0.5}, {0,0,-1}, {0,0}},
	{{0.5,0.5,-0.5}, {0,0,-1}, {1,0}},
	{{0.5,-0.5,-0.5}, {0,0,-1}, {1,1}},
	{{-0.5,-0.5,-0.5}, {0,0,-1}, {0,1}},
	{{0.5,0.5,-0.5}, {1,0,0}, {0,0}},
	{{0.5,0.5,0.5}, {1,0,0}, {1,0}},
	{{0.5,-0.5,0.5}, {1,0,0}, {1,1}},
	{{0.5,-0.5,-0.5}, {1,0,0}, {0,1}},
	{{0.5,0.5,0.5}, {0,0,1}, {1,0}},
	{{-0.5,0.5,0.5}, {0,0,1}, {0,0}},
	{{-0.5,-0.5,0.5}, {0,0,1}, {0,1}},
	{{0.5,-0.5,0.5}, {0,0,1}, {1,1}},
	{{-0.5,0.5,0.5}, {-1,0,0}, {1,0}},
	{{-0.5,0.5,-0.5}, {-1,0,0}, {0,0}},
	{{-0.5,-0.5,-0.5}, {-1,0,0}, {0,1}},
	{{-0.5,-0.5,0.5}, {-1,0,0}, {1,1}},
	{{-0.5,0.5,-0.5}, {0,1,0}, {0,1}},
	{{-0.5,0.5,0.5}, {0,1,0}, {0,0}},
	{{0.5,0.5,0.5}, {0,1,0}, {1,0}},
	{{0.5,0.5,-0.5}, {0,1,0}, {1,1}},
};

const std::vector<uint16_t> indices = {
	0,1,2, 0,2,3,
	4,5,6, 4,6,7,
	8,9,10, 8,10,11,
	12,13,14, 12,14,15,
	16,17,18, 16,18,19,
	20,21,22, 20,22,23,
};

class VulkanGraphics : VulkanResources
{
private:
	VulkanSettings CompareVulkanSettings;
	VulkanWindow Window;
	VulkanRenderer renderer;
	
	Camera camera;

	AmbientLight Ambiant;
	Lighter lighter;

	//ModelLoader modelLoader;
//	Model Nanosuit;

	SkyBox Skybox;
	CubeMapTexture SkyboxTexture;

	Texture2D texture;
	std::vector<Mesh> MeshList;
	//std::vector<Model> ModelList;
	GUIDebugger guiDebugger;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	void InitializeGUIDebugger();
	void UpdateMouse();
	void UpdateKeyboard();
	void Update(uint32_t NextFrameIndex);
	void UpdateCommandBuffers(uint32_t NextFrameIndex);
	void Draw();

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
