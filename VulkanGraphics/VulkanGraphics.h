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

//const std::vector<Vertex> vertices = {
//{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  0.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},
//
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
//	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
//
//	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f,  0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{-0.5f, -0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//
//	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
//
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
//
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}},
//	{{0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  1.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
//	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  0.0f}},
//	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}}
//};
//
//
////const std::vector<Vertex> vertices = {
////	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
////	{{0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
////{ {0.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  1.0f }},
////{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f,  1.0f }},
////
////{ {0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
////{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f,  1.0f }},
////{ {1.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f,  0.0f }}
////};
//const std::vector<uint16_t> indices = {
//
//};

const std::vector<Vertex> vertices =
{
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
	{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
};

const std::vector<uint16_t> indices =
{
	  0, 1, 2, 2, 3, 0
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
