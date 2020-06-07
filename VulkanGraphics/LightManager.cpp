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
			ImGui::SliderFloat3("PointLight0Pos", &PointLightList[0].GetPosPtr()->x, -10.0f, 10.0f);
			ImGui::ColorEdit3("PointLight0Ambient", &PointLightList[0].GetAmbientPtr()->x);
			ImGui::ColorEdit3("PointLight0Diffuse", &PointLightList[0].GetDiffusePtr()->x);
			ImGui::ColorEdit3("PointLight0Specular", &PointLightList[0].GetSpecularPtr()->x);
			ImGui::SliderFloat("PointLight0Constant", PointLightList[0].GetConstantPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("PointLight0Linear", PointLightList[0].GetLinearPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("PointLight0Quadratic", PointLightList[0].GetQuadraticPtr(), 0.0f, 10.0f);
			ImGui::NextColumn();
		}
		else if (selectedNode == 1)
		{
			ImGui::NextColumn();
			ImGui::LabelText("", "Directional");
			ImGui::SliderFloat3("PointLight1Pos", &PointLightList[0].GetPosPtr()->x, -10.0f, 10.0f);
			ImGui::ColorEdit3("PointLight1Ambient", &PointLightList[0].GetAmbientPtr()->x);
			ImGui::ColorEdit3("PointLight1Diffuse", &PointLightList[0].GetDiffusePtr()->x);
			ImGui::ColorEdit3("PointLight1Specular", &PointLightList[0].GetSpecularPtr()->x);
			ImGui::SliderFloat("PointLight1Constant", PointLightList[0].GetConstantPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("PointLight1Linear", PointLightList[0].GetLinearPtr(), 0.0f, 10.0f);
			ImGui::SliderFloat("PointLight1Quadratic", PointLightList[0].GetQuadraticPtr(), 0.0f, 10.0f);
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
