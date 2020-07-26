#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "RendererManager.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "GUIDebugger.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Model.h"
#include "SkyBoxMesh.h"
#include "DebugLightMesh.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "LightManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <chrono>
#include "Texture2D.h"
#include "FrameBufferMesh.h"

const std::vector<Vertex> vertices =
{
      { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {10.0f, 0.0f}},
    		{{10.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    		{{10.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 10.0f}},
    		{{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {10.0f, 10.0f}}
};

const std::vector<uint16_t> indices =
{
    0, 1, 2, 2, 3, 0
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

class VulkanGraphics
{
private:
	//Lights light = {};

	ModelLoader modelLoader;

	//TextureMaps maps;
	VulkanRendererSettings CompareVulkanSettings;
	VulkanWindow Window;
    RendererManager renderer;
    //Texture2D newtexture;
    Texture2D newtexture2;
    //Texture2D MMtexture1;
    //Texture2D MMtexture2;
    //Texture2D MMtexture3;
   // std::vector<Texture2D> TextureList;

	Keyboard keyboard;
	Mouse mouse;

	Camera camera;

	glm::vec3 LightPos;

	//LightManager lightManager;

    CubeMapTexture newtexturebox;
    SkyBoxMesh Skybox;
    FrameBufferMesh frameBuffer;

	std::vector<Mesh2> MeshList;
	//std::vector<Model> ModelList;

    void UpdateImGUI();
	void Update(uint32_t DrawFrame);

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
