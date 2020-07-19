//#pragma once
//#include "Camera.h"
//#include "LightManager.h"
//#include "Skybox.h"
//#include "Model.h"
//
//const std::vector<Vertex> vertices =
//{
//		{ { 10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f,  0.0f}},
//		{ {-10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f,  0.0f}},
//	   { { -10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
//
//	   { {  10.0f, -0.5f,  10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f,  0.0f}},
//	   { { -10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 10.0f}},
//	  { {   10.0f, -0.5f, -10.0f}, {0.0f, 1.0f, 0.0f}, {10.0f, 10.0f}}
//};
//
//const std::vector<uint16_t> indices =
//{
//};
//
//class Scene
//{
//private:
//
//	LightManager lightManager;
//
//	SkyBox Skybox;
//	std::vector<Mesh> MeshList;
//	std::vector<Model> ModelList;
//
//	TextureMaps maps;
//	glm::vec3 LightPos;
//	Lights light = {};
//public:
//	Camera camera;
//
//	Scene();
//	Scene(Renderer& renderer);
//	~Scene();
//
//	void Update(Renderer& renderer);
//	void Draw(Renderer& renderer, uint32_t DrawFrame);
//	void Destroy(Renderer& renderer);
//};
//
