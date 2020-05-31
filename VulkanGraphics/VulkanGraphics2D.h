#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "VulkanResources.h"
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
#include <map>
#include "Level2D.h"

class VulkanGraphics2D : VulkanResources
{
private:

	std::map<Level, TileSet> MapTile;
	Level2D level;

	VulkanRendererSettings CompareVulkanSettings;
	VulkanWindow Window;
	Renderer renderer;

	Keyboard keyboard;
	Mouse mouse;

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
