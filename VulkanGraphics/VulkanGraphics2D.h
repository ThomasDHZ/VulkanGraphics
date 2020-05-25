#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "VulkanRenderer2D.h"
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

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



const std::vector<Vertex> vertices = 
{
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = 
{
	  0, 1, 2, 2, 3, 0
};

class VulkanGraphics2D : VulkanResources
{
private:
	VulkanRendererSettings CompareVulkanSettings;
	VulkanWindow Window;
	VulkanRenderer renderer;

	Keyboard keyboard;
	Mouse mouse;

	Camera camera;

	glm::vec3 LightPos;

	LightManager lightManager;

	Texture2D texture;
	Texture2D texture2;
	std::vector<Mesh> MeshList;

	GUIDebugger guiDebugger;

	void InitializeGUIDebugger();
	void Update(uint32_t DrawFrame);
	void UpdateCommandBuffers(uint32_t DrawFrame);
	void Draw();

public:
	VulkanGraphics2D(int Width, int Height, const char* AppName);
	~VulkanGraphics2D();
	void MainLoop();
};
