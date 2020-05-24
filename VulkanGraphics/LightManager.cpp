#include "LightManager.h"
#include "ImGui/imgui.h"

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
}

void LightManager::Update(VulkanRenderer& Renderer, Camera camera)
{
	for (auto directionalLight : DirectionalLightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		//ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(Renderer)->width / (float)GetSwapChainResolution(Renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		directionalLight.UpdateDebugMesh(Renderer, ubo2);
	}
	for (auto pointLight : PointLightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(Renderer)->width / (float)GetSwapChainResolution(Renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		pointLight.UpdateDebugMesh(Renderer, ubo2);
	}
	for (auto spotlight : SpotlightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		//ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(Renderer)->width / (float)GetSwapChainResolution(Renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		spotlight.UpdateDebugMesh(Renderer, ubo2);
	}
}

void LightManager::UpdateLights()
{
	float PointLight0Pos[3] = { PointLightList[0].GetLightPos()->x, PointLightList[0].GetLightPos()->y, PointLightList[0].GetLightPos()->z };
	float PointLight0Ambient[3] = { PointLightList[0].GetAmbient().x, PointLightList[0].GetAmbient().y, PointLightList[0].GetAmbient().z };
	float PointLight0Diffuse[3] = { PointLightList[0].GetDiffuse().x, PointLightList[0].GetDiffuse().y, PointLightList[0].GetDiffuse().z };
	float PointLight0Specular[3] = { PointLightList[0].GetSpecular().x, PointLightList[0].GetSpecular().y, PointLightList[0].GetSpecular().z }; 

	float PointLight1Pos[3] = { PointLightList[1].GetLightPos()->x, PointLightList[1].GetLightPos()->y, PointLightList[1].GetLightPos()->z };
	float PointLight1Ambient[3] = { PointLightList[1].GetAmbient().x, PointLightList[1].GetAmbient().y, PointLightList[1].GetAmbient().z };
	float PointLight1Diffuse[3] = { PointLightList[1].GetDiffuse().x, PointLightList[1].GetDiffuse().y, PointLightList[1].GetDiffuse().z };
	float PointLight1Specular[3] = { PointLightList[1].GetSpecular().x, PointLightList[1].GetSpecular().y, PointLightList[1].GetSpecular().z };

	float PointLight2Pos[3] = { PointLightList[2].GetLightPos()->x, PointLightList[2].GetLightPos()->y, PointLightList[2].GetLightPos()->z };
	float PointLight2Ambient[3] = { PointLightList[2].GetAmbient().x, PointLightList[2].GetAmbient().y, PointLightList[2].GetAmbient().z };
	float PointLight2Diffuse[3] = { PointLightList[2].GetDiffuse().x, PointLightList[2].GetDiffuse().y, PointLightList[2].GetDiffuse().z };
	float PointLight2Specular[3] = { PointLightList[2].GetSpecular().x, PointLightList[2].GetSpecular().y, PointLightList[2].GetSpecular().z };

	float PointLight3Pos[3] = { PointLightList[3].GetLightPos()->x, PointLightList[3].GetLightPos()->y, PointLightList[3].GetLightPos()->z };
	float PointLight3Ambient[3] = { PointLightList[3].GetAmbient().x, PointLightList[3].GetAmbient().y, PointLightList[3].GetAmbient().z };
	float PointLight3Diffuse[3] = { PointLightList[3].GetDiffuse().x, PointLightList[3].GetDiffuse().y, PointLightList[3].GetDiffuse().z };
	float PointLight3Specular[3] = { PointLightList[3].GetSpecular().x, PointLightList[3].GetSpecular().y, PointLightList[3].GetSpecular().z };

	//ImGui::Begin("Settings2");
	//if (ImGui::TreeNode("Light Tree"))
	//{
	//	int x = 0;
	//	for (auto pointLight : PointLightList)
	//	{
	//		ImGui::Columns(2, "tree", true);
	//		x++;
	//		if (ImGui::TreeNode("PointLight" + x))
	//		{
	//			
	//			ImGui::NextColumn();
	//			ImGui::LabelText("", "Directional");
	//			ImGui::SliderFloat3("PointLight0Pos", PointLight0Pos, -10.0f, 10.0f);
	//			ImGui::ColorEdit3("PointLight0Ambient", PointLight0Ambient);
	//			ImGui::ColorEdit3("PointLight0Diffuse", PointLight0Diffuse);
	//			ImGui::ColorEdit3("PointLight0Specular", PointLight0Specular);
	//			ImGui::NextColumn();
	//			ImGui::TreePop();
	//		}
	//	}
	//	ImGui::SliderFloat3("PointLights 1", PointLight1, -10.0f, 10.0f);
	//	ImGui::SliderFloat3("PointLights 2", PointLight2, -10.0f, 10.0f);
	//	ImGui::SliderFloat3("PointLights 3", PointLight3, -10.0f, 10.0f);
	//	ImGui::TreePop();
	//}
	//ImGui::End();


		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static int selectedNode = -1; // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.

		ImGui::Columns(2, "tree", true);
		for (int i = 0; i < 4; i++)
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			if (selectedNode == i)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
			}

		}
		if (node_clicked != -1)
		{
			selectedNode = node_clicked;
		}

		if (selectedNode == 0)
		{
			ImGui::NextColumn();
			ImGui::LabelText("", "Directional");
			ImGui::SliderFloat3("PointLight0Pos", PointLight0Pos, -10.0f, 10.0f);
			ImGui::ColorEdit3("PointLight0Ambient", PointLight0Ambient);
			ImGui::ColorEdit3("PointLight0Diffuse", PointLight0Diffuse);
			ImGui::ColorEdit3("PointLight0Specular", PointLight0Specular);
			ImGui::NextColumn();
		}
		else if (selectedNode == 1)
		{
			ImGui::NextColumn();
			ImGui::LabelText("", "Directional2");
			ImGui::SliderFloat3("PointLight1Pos", PointLight1Pos, -10.0f, 10.0f);
			ImGui::ColorEdit3("PointLight1Ambient", PointLight1Ambient);
			ImGui::ColorEdit3("PointLight1Diffuse", PointLight1Diffuse);
			ImGui::ColorEdit3("PointLight1Specular", PointLight1Specular);
			ImGui::NextColumn();
		}

	PointLightList[0].SetPosition(PointLight0Pos);
	PointLightList[0].SetAmbient(PointLight0Ambient);
	PointLightList[0].SetDiffuse(PointLight0Diffuse);
	PointLightList[0].SetSpecular(PointLight0Specular);
	PointLightList[1].SetPosition(PointLight1Pos);
	PointLightList[1].SetAmbient(PointLight1Ambient);
	PointLightList[1].SetDiffuse(PointLight1Diffuse);
	PointLightList[1].SetSpecular(PointLight1Specular);
	//PointLightList[1].SetPosition(PointLight1);
	//PointLightList[2].SetPosition(PointLight2);
	//PointLightList[3].SetPosition(PointLight3);
}

void LightManager::DrawDebugMesh(VulkanRenderer& Renderer, uint32_t DrawFrame)
{
	for (auto directionalLight : DirectionalLightList)
	{
		directionalLight.DrawDebugMesh(Renderer, DrawFrame);
	}
	for (auto pointLight : PointLightList)
	{
		pointLight.DrawDebugMesh(Renderer, DrawFrame);
	}
	for (auto spotlight : SpotlightList)
	{
		spotlight.DrawDebugMesh(Renderer, DrawFrame);
	}
}

void LightManager::Destroy(VulkanRenderer& Renderer)
{
	for (auto  directionalLight : DirectionalLightList)
	{
		directionalLight.Destroy(Renderer);
	}
	for (auto pointLight : PointLightList)
	{
		pointLight.Destroy(Renderer);
	}
	for (auto spotlight : SpotlightList)
	{
		spotlight.Destroy(Renderer);
	}
}
