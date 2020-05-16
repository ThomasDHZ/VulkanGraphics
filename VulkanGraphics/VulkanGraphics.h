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


const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
	{{0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},

		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},

		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},

		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},

		{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}},

		{{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}}
};

const std::vector<uint16_t> indices = {

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

	ModelLoader modelLoader;
	Model Nanosuit;

	SkyBox Skybox;
	CubeMapTexture SkyboxTexture;

	Texture2D texture;
	Mesh MeshList;
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
