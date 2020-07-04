#include "Level2D.h"
#include "VulkanGraphics.h"

Level2D::Level2D()
{
}

Level2D::Level2D(Renderer& renderer, TileSet tileset)
{
	camera = Camera(glm::vec3(0.0f, 6.0f, 10.0f));
	LevelMap = LevelMesh2D(renderer, tileset);


	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	TextureMaps TextureList = {};
	TextureList.DiffuseMap = Texture2D(renderer, tileset.DiffuseMap);
	TextureList.SpecularMap = Texture2D(renderer, tileset.SpecularMap);
	TextureList.NormalMap = Texture2D(renderer, tileset.NormalMap);
	TextureList.DisplacementMap = Texture2D(renderer, tileset.AlphaMap);
	TextureList.AlphaMap = Texture2D(renderer, tileset.AlphaMap);
	TextureList.CubeMap = CubeMapTexture(renderer, layout);

	SpriteList = Sprite(renderer, glm::vec2(3.33f, 8.5f));
    //ColliderSprite = Mesh(renderer, LevelMap.VertexList, LevelMap.IndexList, TextureList);
	//ColliderSprite.MeshPosition = glm::vec3(5.0f, 8.5, 0.0f);


	DirectionalLightBuffer lighter;
	lighter.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lighter.Ambient = glm::vec3(0.0, 0.0, 0.0);
	lighter.Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	lighter.Specular = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLightBuffer light1;
	light1.Position = glm::vec3(4.737f, 6.0f, 4.0f);
	light1.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light1.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light1.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.Constant = 1.0f;
	light1.Linear = 0.09f;
	light1.Quadratic = 0.032;

	PointLightBuffer light2;
	light2.Position = glm::vec3(9.737f, 6.0f, 4.0f);
	light2.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light2.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light2.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light2.Constant = 1.0f;
	light2.Linear = 0.09f;
	light2.Quadratic = 0.032;

	PointLightBuffer light3;
	light3.Position = glm::vec3(13.737f, 6.0f, 4.0f);
	light3.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light3.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light3.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light3.Constant = 1.0f;
	light3.Linear = 0.09f;
	light3.Quadratic = 0.032;

	PointLightBuffer light4;
	light4.Position = glm::vec3(17.737f, 6.0f, 4.0f);
	light4.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light4.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light4.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light4.Constant = 1.0f;
	light4.Linear = 0.09f;
	light4.Quadratic = 0.032;

	SpotLightBuffer spotLight = {};
	spotLight.Position = camera.Position;
	spotLight.Direction = camera.Front;
	spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Constant = 1.0f;
	spotLight.Linear = 0.09f;
	spotLight.Quadratic = 0.032f;
	spotLight.CutOff = glm::cos(glm::radians(12.5f));
	spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));

	lightManager.DirectionalLightList.emplace_back(DirectionalLight(renderer, lighter));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light1));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light2));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light3));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light4));
	lightManager.SpotlightList.emplace_back(SpotLight(renderer, spotLight));
}

Level2D::~Level2D()
{
}

void Level2D::LevelDebug(Renderer& renderer)
{
	ImGui::Begin("Settings");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Mesh View", &renderer.Settings.ShowMeshLines);
	ImGui::Checkbox("Collision View", &renderer.Settings.ShowDebugCollisionMesh);
	ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
	ImGui::Checkbox("2D Mode", &renderer.Settings.TwoDMode);
	//ImGui::SliderFloat3("Camera", camera.GetCameraPosPtr(), -10.0f, 10.0f);
	ImGui::SliderFloat3("Sprite", SpriteList.SpriteMesh.GetMeshPosPtr(), -10.0f, 10.0f);
	ImGui::End();

	lightManager.UpdateLights();
}

void Level2D::LevelLoop(Renderer& renderer)
{
}

void Level2D::Update(Renderer& renderer, GLFWwindow* Window)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	Material material = {};
	material.Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.Shininess = 32.0f;

	Lights viewing = {};
	viewing.directionalLight = lightManager.DirectionalLightList[0].GetSettings();
	viewing.pointLight = lightManager.PointLightList[0].GetSettings();
	//viewing.pointLight[1] = lightManager.PointLightList[1].GetSettings();
	//viewing.pointLight[2] = lightManager.PointLightList[2].GetSettings();
	//viewing.pointLight[3] = lightManager.PointLightList[3].GetSettings();
	viewing.spotLight = lightManager.SpotlightList[0].GetSettings();
	viewing.viewPos = camera.Position;

	MeshProperties meshProperties = {};
	meshProperties.material = material;
	meshProperties.SpriteUV = glm::vec2(0.0f, 0.0f);
	//viewing.timer = time;

	PositionMatrix ubo{};
	ubo.model = glm::mat4(1.0f);
	ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, -4.0f, 0.0f));
	ubo.view = camera.GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;
	ubo.timer = glfwGetTime();

	Lights light = {};
	light.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	light.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
	light.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
	light.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
	light.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	light.pointLight.Ambient = glm::vec3(0.005f);
	light.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light.pointLight.Specular = glm::vec3(0.3f);
	light.pointLight.Constant = 1.0f;
	light.pointLight.Linear = 0.09f;
	light.pointLight.Quadratic = 0.032f;
	light.spotLight.Position = camera.Position;
	light.spotLight.Direction = camera.Front;
	light.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	light.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Constant = 1.0f;
	light.spotLight.Linear = 0.09f;
	light.spotLight.Quadratic = 0.032f;
	light.spotLight.CutOff = glm::cos(glm::radians(12.5f));
	light.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
	light.viewPos = camera.Position;

	PositionMatrix ubo3{};
	ubo3.model = glm::mat4(1.0f);
	ubo3.model = glm::translate(ubo.model, SpriteList.SpriteMesh.MeshPosition);
	ubo3.view = camera.GetViewMatrix();
	ubo3.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	ubo3.proj[1][1] *= -1;
	ubo3.timer = glfwGetTime();


	//ColliderSprite.Update(renderer, camera, light);

	LevelMap.Update(renderer, camera, light);
	SpriteList.Update(Window, renderer, camera, light);
	lightManager.Update(renderer, camera);
}

void Level2D::Draw(Renderer& renderer, uint32_t DrawFrame)
{

	LevelMap.Draw(renderer, DrawFrame);
	SpriteList.Draw(renderer, DrawFrame);
//	ColliderSprite.Draw(renderer, DrawFrame);
	if (renderer.Settings.ShowDebugLightMesh)
	{
		lightManager.DrawDebugMesh(renderer, DrawFrame);
	}
	tempflag = true;
}

void Level2D::Destroy(Renderer& renderer)
{
	//ColliderSprite.Destory(renderer);
	SpriteList.Destory(renderer);
	lightManager.Destroy(renderer);
	LevelMap.Destory(renderer);
}
