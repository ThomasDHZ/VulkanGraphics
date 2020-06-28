#include "Mesh.h"
#include "ImGui/imgui.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(Renderer& renderer) : BaseMesh(renderer)
{
}

Mesh::Mesh(Renderer& renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, const TextureMaps& textureList) : BaseMesh(renderer, vertexList, indexList, textureList)
{
	CreateMaterialProperties(textureList);
	CreateUniformBuffers(renderer);
	CreateDescriptorPool(renderer);
	CreateDescriptorSets(renderer);
	CalcTangent();
}

Mesh::Mesh(Renderer& renderer, const TextureMaps& textureList) : BaseMesh(renderer, textureList)
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateMaterialProperties(const TextureMaps& textureList)
{
	properites.material.Diffuse = glm::vec3(0.0f);
	properites.material.Specular = glm::vec3(1.0f);
	properites.material.Shininess = 32;
	properites.material.Alpha = 0;
	properites.material.reflection = 0;
	properites.SpriteUV = glm::vec2(0.0f);


	properites.MapFlags.DiffuseMapFlag = 1;
	properites.MapFlags.SpecularMapFlag = 1;
	properites.MapFlags.NormalMapFlag = 1;
	properites.MapFlags.DisplacementMapFlag = 1;
	properites.MapFlags.AlphaMapFlag = 1;
	properites.MapFlags.CubeMapFlag = 1;

	if (textureList.DiffuseMap.Width == 1 &&
		textureList.DiffuseMap.Width == 1)
	{
		properites.MapFlags.DiffuseMapFlag = 0;
	}

	if (textureList.SpecularMap.Width == 1 &&
		textureList.SpecularMap.Width == 1)
	{
		properites.MapFlags.SpecularMapFlag = 0;
	}

	if (textureList.NormalMap.Width == 1 &&
		textureList.NormalMap.Width == 1)
	{
		properites.MapFlags.NormalMapFlag = 0;
	}

	if (textureList.DisplacementMap.Width == 1 &&
		textureList.DisplacementMap.Width == 1)
	{
		properites.MapFlags.DisplacementMapFlag = 0;
	}

	if (textureList.AlphaMap.Width == 1 &&
		textureList.AlphaMap.Width == 1)
	{
		properites.MapFlags.AlphaMapFlag = 0;
	}

	if (textureList.CubeMap.Width == 1 &&
		textureList.CubeMap.Width == 1)
	{
		properites.MapFlags.CubeMapFlag = 0;
	}
}

void Mesh::CreateUniformBuffers(Renderer& renderer)
{
	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(PositionMatrix));
	MeshPropertiesBuffer = UniformBuffer(renderer, sizeof(MeshProperties));
	LightsBuffer = UniformBuffer(renderer, sizeof(Lights));
}

void Mesh::CreateDescriptorPool(Renderer& renderer)
{
	std::array<DescriptorPoolSizeInfo, 8>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(Renderer& renderer)
{
	BaseMesh::CreateDescriptorSets(renderer, *GetDescriptorSetLayout(renderer));

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList.DiffuseMap.textureImageView;
	DiffuseMap.sampler = TextureList.DiffuseMap.textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureList.SpecularMap.textureImageView;
	SpecularMap.sampler = TextureList.SpecularMap.textureSampler;

	VkDescriptorImageInfo NormalMap = {};
	NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	NormalMap.imageView = TextureList.NormalMap.textureImageView;
	NormalMap.sampler = TextureList.NormalMap.textureSampler;

	VkDescriptorImageInfo DisplacementMap = {};
	DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DisplacementMap.imageView = TextureList.DisplacementMap.textureImageView;
	DisplacementMap.sampler = TextureList.DisplacementMap.textureSampler;

	VkDescriptorImageInfo AlphaMap = {};
	AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	AlphaMap.imageView = TextureList.AlphaMap.textureImageView;
	AlphaMap.sampler = TextureList.AlphaMap.textureSampler;

	//VkDescriptorImageInfo CubeMap = {};
	//CubeMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//CubeMap.imageView = TextureList.CubeMap.textureImageView;
	//CubeMap.sampler = TextureList.CubeMap.textureSampler;

	for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
	{
		VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		VkDescriptorBufferInfo ViewPosInfo = {};
		ViewPosInfo.buffer = MeshPropertiesBuffer.GetUniformBuffer(i);
		ViewPosInfo.offset = 0;
		ViewPosInfo.range = sizeof(MeshProperties);

		VkDescriptorBufferInfo LightsInfo = {};
		LightsInfo.buffer = LightsBuffer.GetUniformBuffer(i);
		LightsInfo.offset = 0;
		LightsInfo.range = sizeof(Lights);

		std::vector<WriteDescriptorSetInfo> DescriptorList;

		WriteDescriptorSetInfo PositionDescriptor;
		PositionDescriptor.DstBinding = 0;
		PositionDescriptor.DstSet = descriptorSets[i];
		PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		PositionDescriptor.DescriptorBufferInfo = PositionInfo;
		DescriptorList.emplace_back(PositionDescriptor);

		WriteDescriptorSetInfo DiffuseMapDescriptor;
		DiffuseMapDescriptor.DstBinding = 1;
		DiffuseMapDescriptor.DstSet = descriptorSets[i];
		DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DiffuseMapDescriptor.DescriptorImageInfo = DiffuseMap;
		DescriptorList.emplace_back(DiffuseMapDescriptor);

		WriteDescriptorSetInfo SpecularMapDescriptor;
		SpecularMapDescriptor.DstBinding = 2;
		SpecularMapDescriptor.DstSet = descriptorSets[i];
		SpecularMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		SpecularMapDescriptor.DescriptorImageInfo = SpecularMap;
		DescriptorList.emplace_back(SpecularMapDescriptor);

		WriteDescriptorSetInfo NormalMapDescriptor;
		NormalMapDescriptor.DstBinding = 3;
		NormalMapDescriptor.DstSet = descriptorSets[i];
		NormalMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		NormalMapDescriptor.DescriptorImageInfo = NormalMap;
		DescriptorList.emplace_back(NormalMapDescriptor);

		WriteDescriptorSetInfo DisplacementMapDescriptor;
		DisplacementMapDescriptor.DstBinding = 4;
		DisplacementMapDescriptor.DstSet = descriptorSets[i];
		DisplacementMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DisplacementMapDescriptor.DescriptorImageInfo = DisplacementMap;
		DescriptorList.emplace_back(DisplacementMapDescriptor);

		WriteDescriptorSetInfo AlphaMapDescriptor;
		AlphaMapDescriptor.DstBinding = 5;
		AlphaMapDescriptor.DstSet = descriptorSets[i];
		AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
		DescriptorList.emplace_back(AlphaMapDescriptor);

		//WriteDescriptorSetInfo CubeMapDescriptor;
		//CubeMapDescriptor.DstBinding = 6;
		//CubeMapDescriptor.DstSet = descriptorSets[i];
		//CubeMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//CubeMapDescriptor.DescriptorImageInfo = CubeMap;
		//DescriptorList.emplace_back(CubeMapDescriptor);

		WriteDescriptorSetInfo ViewPosDescriptor;
		ViewPosDescriptor.DstBinding = 7;
		ViewPosDescriptor.DstSet = descriptorSets[i];
		ViewPosDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		ViewPosDescriptor.DescriptorBufferInfo = ViewPosInfo;
		DescriptorList.emplace_back(ViewPosDescriptor);

		WriteDescriptorSetInfo LightsDescriptor;
		LightsDescriptor.DstBinding = 8;
		LightsDescriptor.DstSet = descriptorSets[i];
		LightsDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		LightsDescriptor.DescriptorBufferInfo = LightsInfo;
		DescriptorList.emplace_back(LightsDescriptor);

		Mesh::CreateDescriptorSetsData(renderer, DescriptorList);
	}
}

void Mesh::CalcTangent()
{/*
	{ {-1, -1, 0.0f}, { 0.0f, 0.0f, 1.0f }, { 1, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }},
	{ {1, -1, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
	{ {1, 1, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
	{ {-1, 1, 0.0f}, {0.0f, 0.0f, 1.0f}, {1, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }*/

	glm::vec3 tangent;
	glm::vec3 bitangent;

	for (int x = 0; x < IndexList.size(); x += 3)
	{
		int index = IndexList[x];
		int index1 = IndexList[x + 1];
		int index2 = IndexList[x + 2];

		glm::vec3 edge = VertexList[index1].Position - VertexList[index].Position;
		glm::vec3 edge2 = VertexList[index2].Position - VertexList[index].Position;
		glm::vec2 deltaUV = VertexList[index1].TexureCoord - VertexList[index].TexureCoord;
		glm::vec2 deltaUV2 = VertexList[index2].TexureCoord - VertexList[index].TexureCoord;

		float f = 1.0f / (deltaUV.x * deltaUV2.y - deltaUV2.x * deltaUV.y);

		tangent.x = f * (deltaUV2.y * edge.x - deltaUV.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge.y - deltaUV.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge.z - deltaUV.y * edge2.z);

		bitangent.x = f * (-deltaUV2.x * edge.x + deltaUV.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge.y + deltaUV.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge.z + deltaUV.x * edge2.z);

		VertexList[index].Tangant = glm::vec3(tangent.x, tangent.y, tangent.z);
		VertexList[index1].Tangant = glm::vec3(tangent.x, tangent.y, tangent.z);
		VertexList[index2].Tangant = glm::vec3(tangent.x, tangent.y, tangent.z);
		VertexList[index].BiTangant = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
		VertexList[index1].BiTangant = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
		VertexList[index2].BiTangant = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
	}
}

void Mesh::Update(Renderer& renderer, Camera& camera, Lights light)
{
	PositionMatrix ubo{};
	ubo.model = glm::mat4(1.0f);
	ubo.model = glm::translate(ubo.model, MeshPosition);
	ubo.model = glm::scale(ubo.model, MeshScale);
	ubo.view = camera.GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 10000.0f);
	ubo.proj[1][1] *= -1;

	if (RotationAmount != 0 &&
		(MeshRotate.x != 0 ||
		 MeshRotate.y != 0 ||
		 MeshRotate.z != 0))
	{
		ubo.model = glm::rotate(ubo.model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	}

	UpdateUniformBuffer(renderer, ubo, light);
}

void Mesh::UpdateGUI()
{
	ImGui::Begin("Mesh");
	ImGui::Columns(3, "Lights", true);
	ImGui::SliderFloat3("Position", &MeshPosition.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Rotation", &MeshRotate.x, -1.0f, 1.0f);
	ImGui::SliderFloat3("Scale", &MeshScale.x, 0.0f, 50.0f);
	ImGui::SliderFloat("Rotate", &RotationAmount, 0.0f, 50.0f);
	ImGui::SliderFloat("Shininess", &properites.material.Shininess, 0.0f, 50.0f);
	ImGui::SliderFloat("Reflection", &properites.material.reflection, 0.0f, 1.0f);
	ImGui::SliderFloat2("UV Offset", &properites.SpriteUV.x, 0.0f, 3.0f);
	ImGui::NextColumn();

	ImGui::SliderInt("DiffuseMap", &properites.MapFlags.DiffuseMapFlag, 0, 1);
	ImGui::SliderInt("SpecularMap", &properites.MapFlags.SpecularMapFlag, 0, 1);
	ImGui::SliderInt("NormalMap", &properites.MapFlags.NormalMapFlag, 0, 1);
	ImGui::SliderInt("DisplacementMap", &properites.MapFlags.DisplacementMapFlag, 0, 1);
	ImGui::SliderInt("AlphaMap", &properites.MapFlags.AlphaMapFlag, 0, 1);
	ImGui::SliderInt("RelectionCubeMap", &properites.MapFlags.CubeMapFlag, 0, 1);

	ImGui::NextColumn();
	if (properites.MapFlags.DiffuseMapFlag == 0)
	{
		ImGui::ColorPicker3("Diffuse", &properites.material.Diffuse.x);
	}
	if (properites.MapFlags.SpecularMapFlag == 0)
	{
		ImGui::ColorPicker3("Specular", &properites.material.Specular.x);
	}

	ImGui::NextColumn();

	ImGui::End();
}

void Mesh::Draw(Renderer& renderer, int currentFrame)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if(renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetMeshViewShaderPipeline(renderer));
		vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		if (IndiceSize == 0)
		{
			vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
		}
		else
		{
			vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
		}
	}

	if (renderer.Settings.ShowDebugCollisionMesh)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetCollisionShaderPipeline(renderer));
		vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
		vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		if (IndiceSize == 0)
		{
			vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
		}
		else
		{
			vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
		}
	}

	vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(renderer));
	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	if(IndiceSize == 0)
	{
		vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
	}
}

void Mesh::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, Lights light)
{
	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
	MeshPropertiesBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&properites));
	LightsBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&light));
}

void Mesh::Destroy(Renderer& renderer)
{
	PositionMatrixBuffer.Destroy(renderer);
	MeshPropertiesBuffer.Destroy(renderer);
	LightsBuffer.Destroy(renderer);

	BaseMesh::Destory(renderer);
}

