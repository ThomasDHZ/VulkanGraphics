#include "LightManager.h"
#include "ImGui/imgui.h"

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
}

void LightManager::Update(Renderer& renderer, Camera camera)
{
	for (auto directionalLight : DirectionalLightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		//ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		directionalLight.UpdateDebugMesh(renderer, ubo2);
	}
	for (auto pointLight : PointLightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		pointLight.UpdateDebugMesh(renderer, ubo2);
	}
	for (auto spotlight : SpotlightList)
	{
		PositionMatrix ubo2{};
		ubo2.model = glm::mat4(1.0f);
		//ubo2.model = glm::translate(ubo2.model, pointLight.GetPosition());
		ubo2.model = glm::scale(ubo2.model, glm::vec3(.2f));
		ubo2.view = camera.GetViewMatrix();
		ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
		ubo2.proj[1][1] *= -1;

		spotlight.UpdateDebugMesh(renderer, ubo2);
	}
}

void LightManager::UpdateLights()
{
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static int selectedNode = -1; 
		int node_clicked = -1;              

		ImGui::Columns(2, "Lights", true);
		for (int i = 0; i < 4; i++)
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			if (selectedNode == i)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Point Light %d", i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
			}

		}
		if (node_clicked != -1)
		{
			selectedNode = node_clicked;
		}

		if (selectedNode != -1)
		{
			ImGui::NextColumn();
			ImGui::LabelText("", "Directional");
			ImGui::SliderFloat3("Position", &PointLightList[selectedNode].GetPosPtr()->x, -10.0f, 10.0f);
			ImGui::ColorEdit3("Ambient", &PointLightList[selectedNode].GetAmbientPtr()->x);
			ImGui::ColorEdit3("Diffuse", &PointLightList[selectedNode].GetDiffusePtr()->x);
			ImGui::ColorEdit3("Specular", &PointLightList[selectedNode].GetSpecularPtr()->x);
			ImGui::SliderFloat("Constant", PointLightList[selectedNode].GetConstantPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("Linear", PointLightList[selectedNode].GetLinearPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("Quadratic", PointLightList[selectedNode].GetQuadraticPtr(), 0.0f, 10.0f);
			ImGui::NextColumn();
		}
}

void LightManager::DrawDebugMesh(Renderer& renderer, uint32_t DrawFrame)
{
	for (auto directionalLight : DirectionalLightList)
	{
		directionalLight.DrawDebugMesh(renderer, DrawFrame);
	}
	for (auto pointLight : PointLightList)
	{
		pointLight.DrawDebugMesh(renderer, DrawFrame);
	}
	for (auto spotlight : SpotlightList)
	{
		spotlight.DrawDebugMesh(renderer, DrawFrame);
	}
}

void LightManager::Destroy(Renderer& renderer)
{
	for (auto  directionalLight : DirectionalLightList)
	{
		directionalLight.Destroy(renderer);
	}
	for (auto pointLight : PointLightList)
	{
		pointLight.Destroy(renderer);
	}
	for (auto spotlight : SpotlightList)
	{
		spotlight.Destroy(renderer);
	}
}
