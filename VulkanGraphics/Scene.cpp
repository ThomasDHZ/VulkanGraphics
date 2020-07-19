//#include "Scene.h"
//
//Scene::Scene()
//{
//}
//
//Scene::Scene(Renderer& renderer)
//{
//	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
//
//	CubeMapLayout layout;
//	layout.Left = "texture/skybox/left.jpg";
//	layout.Right = "texture/skybox/right.jpg";
//	layout.Top = "texture/skybox/top.jpg";
//	layout.Bottom = "texture/skybox/bottom.jpg";
//	layout.Back = "texture/skybox/back.jpg";
//	layout.Front = "texture/skybox/front.jpg";
//
//	maps.DiffuseMap = Texture2D(renderer, "texture/zxc_diffuseOriginal.bmp");
//	maps.NormalMap = Texture2D(renderer, "texture/Temp.bmp");
//	maps.DisplacementMap = Texture2D(renderer, "texture/Temp.bmp");
//	maps.SpecularMap = Texture2D(renderer, "texture/Temp.bmp");
//	maps.AlphaMap = Texture2D(renderer, "texture/Temp.bmp");
//	maps.CubeMap = CubeMapTexture(renderer, layout);
//
//	Skybox = SkyBox(renderer, maps.CubeMap);
//	MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
//	//ModelList.emplace_back(Model(renderer, modelLoader.GetModelMeshs()));
//
//	LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
//}
//
//Scene::~Scene()
//{
//
//}
//
//void Scene::Update(Renderer& renderer)
//{
//	light.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
//	light.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
//	light.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
//	light.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
//	light.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
//	light.pointLight.Ambient = glm::vec3(0.005f);
//	light.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
//	light.pointLight.Specular = glm::vec3(0.3f);
//	light.pointLight.Constant = 1.0f;
//	light.pointLight.Linear = 0.09f;
//	light.pointLight.Quadratic = 0.032f;
//	light.spotLight.Position = camera.Position;
//	light.spotLight.Direction = camera.Front;
//	light.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
//	light.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
//	light.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
//	light.spotLight.Constant = 1.0f;
//	light.spotLight.Linear = 0.09f;
//	light.spotLight.Quadratic = 0.032f;
//	light.spotLight.CutOff = glm::cos(glm::radians(12.5f));
//	light.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
//	light.viewPos = camera.Position;
//
//	for (auto mesh : MeshList)
//	{
//		mesh.Update(renderer, camera, light);
//	}
//	for (auto model : ModelList)
//	{
//		model.UpdateUniformBuffer(renderer, camera, light);
//	}
//	Skybox.UpdateUniformBuffer(renderer, camera);
//}
//
//void Scene::Draw(Renderer& renderer, uint32_t DrawFrame)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.Draw(renderer, DrawFrame);
//	}
//	for (auto model : ModelList)
//	{
//		model.Draw(renderer, DrawFrame);
//	}
//	if (renderer.Settings.ShowDebugLightMesh)
//	{
//		lightManager.DrawDebugMesh(renderer, DrawFrame);
//	}
//	if (renderer.Settings.ShowSkyBox)
//	{
//		Skybox.Draw(renderer, DrawFrame);
//	}
//}
//
//void Scene::Destroy(Renderer& renderer)
//{
//	maps.DiffuseMap.Destroy(renderer);
//	maps.NormalMap.Destroy(renderer);
//	maps.DisplacementMap.Destroy(renderer);
//	maps.SpecularMap.Destroy(renderer);
//	maps.AlphaMap.Destroy(renderer);
//	maps.CubeMap.Destroy(renderer);
//
//	for (auto mesh : MeshList)
//	{
//		mesh.Destroy(renderer);
//	}
//	lightManager.Destroy(renderer);
//
//	Skybox.Destory(renderer);
//}
