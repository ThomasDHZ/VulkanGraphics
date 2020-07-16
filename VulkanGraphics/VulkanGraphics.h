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
#include "EngineRenderedTexture.h"
#include "Mesh2.h"
#include "NewTexture2D.h"

const std::vector<Vertex> vertices = {
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

const std::vector<Vertex> quadvertices = {
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

const std::vector<uint16_t> quadindices = {
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
	Lights light = {};

	ModelLoader modelLoader;
    EngineRenderedTexture rendereredTexture;
    NewTexture2D texture;
	TextureMaps maps;
	VulkanRendererSettings CompareVulkanSettings;
	VulkanWindow Window;
	Renderer renderer;
	
	Keyboard keyboard;
	Mouse mouse;

	Camera camera;

	glm::vec3 LightPos;

	LightManager lightManager;

	SkyBox Skybox;

	//FrameBufferMesh framebuffer;


    Mesh2 mesh;
    Mesh2 quad;

	void Update(uint32_t DrawFrame);
	void UpdateCommandBuffers(uint32_t DrawFrame);

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
