#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "VulkanResources.h"
#include "GUIDebugger.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Skybox.h"
#include "DebugLightMesh.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "LightManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <chrono>

const std::vector<Vertex> vertices =
{
	{{-1.0,-1.0,-1.0}, {0,1,0}, {0,1}},
	{{1.0,-1.0,-1.0}, {0,1,0}, {1,1}},
	{{1.0,-1.0,1.0}, {0,1,0}, {1,0}},
	{{-1.0,-1.0,1.0}, {0,1,0}, {0,0}},
	{{-1.0,1.0,-1.0}, {0,0,1}, {0,0}},
	{{1.0,1.0,-1.0}, {0,0,1}, {1,0}},
	{{1.0,-1.0,-1.0}, {0,0,1}, {1,1}},
	{{-1.0,-1.0,-1.0}, {0,0,1}, {0,1}},
	{{1.0,1.0,-1.0}, {-1,0,0}, {0,0}},
	{{1.0,1.0,1.0}, {-1,0,0}, {1,0}},
	{{1.0,-1.0,1.0}, {-1,0,0}, {1,1}},
	{{1.0,-1.0,-1.0}, {-1,0,0}, {0,1}},
	{{1.0,1.0,1.0}, {0,0,-1}, {1,0}},
	{{-1.0,1.0,1.0}, {0,0,-1}, {0,0}},
	{{-1.0,-1.0,1.0}, {0,0,-1}, {0,1}},
	{{1.0,-1.0,1.0}, {0,0,-1}, {1,1}},
	{{-1.0,1.0,1.0}, {1,0,0}, {1,0}},
	{{-1.0,1.0,-1.0}, {1,0,0}, {0,0}},
	{{-1.0,-1.0,-1.0}, {1,0,0}, {0,1}},
	{{-1.0,-1.0,1.0}, {1,0,0}, {1,1}},
	{{-1.0,1.0,-1.0}, {0,-1,0}, {0,1}},
	{{-1.0,1.0,1.0}, {0,-1,0}, {0,0}},
	{{1.0,1.0,1.0}, {0,-1,0}, {1,0}},
	{{1.0,1.0,-1.0}, {0,-1,0}, {1,1}},
};

const std::vector<uint16_t> indices =
{
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
	TextureMaps maps;
	VulkanRendererSettings CompareVulkanSettings;
	VulkanWindow Window;
	Renderer renderer;
	
	Keyboard keyboard;
	Mouse mouse;

	Camera camera;

	glm::vec3 LightPos;

	LightManager lightManager;


	//ModelLoader modelLoader;
	//Model Nanosuit;

	SkyBox Skybox;

	std::vector<Mesh> MeshList;
	//std::vector<Model> ModelList;
	GUIDebugger guiDebugger;

	void CalcTangent();
	void InitializeGUIDebugger();
	void Update(uint32_t DrawFrame);
	void UpdateCommandBuffers(uint32_t DrawFrame);
	void Draw();

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
