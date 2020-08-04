#include "Mesh.h"
#include "Texture2D.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture tex, Texture tex2, Texture tex3, CubeMapTexture cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit) : BaseMesh(renderBit)
{
    texture = tex;

    VertexSize = vertexdata.size();
    IndexSize = indicesdata.size();

    CreateVertexBuffer(renderer, vertexdata);

    if (IndexSize != 0)
    {
        CreateIndexBuffer(renderer, indicesdata);
    }

    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, tex2, tex3, cubemap, descriptorSetLayout);
}

Mesh::~Mesh()
{
}

//void Mesh2::CreateMaterialProperties(const TextureMaps& textureList)
//{
//	properites.material.Diffuse = glm::vec3(0.0f);
//	properites.material.Specular = glm::vec3(1.0f);
//	properites.material.Shininess = 32;
//	properites.material.Alpha = 0;
//	properites.material.reflection = 0;
//	properites.SpriteUV = glm::vec2(0.0f);
//
//
//	properites.MapFlags.DiffuseMapFlag = 1;
//	properites.MapFlags.SpecularMapFlag = 1;
//	properites.MapFlags.NormalMapFlag = 1;
//	properites.MapFlags.DisplacementMapFlag = 1;
//	properites.MapFlags.AlphaMapFlag = 1;
//	properites.MapFlags.CubeMapFlag = 1;
//
//	if (textureList.DiffuseMap.Width == 1 &&
//		textureList.DiffuseMap.Width == 1)
//	{
//		properites.MapFlags.DiffuseMapFlag = 0;
//	}
//
//	if (textureList.SpecularMap.Width == 1 &&
//		textureList.SpecularMap.Width == 1)
//	{
//		properites.MapFlags.SpecularMapFlag = 0;
//	}
//
//	if (textureList.NormalMap.Width == 1 &&
//		textureList.NormalMap.Width == 1)
//	{
//		properites.MapFlags.NormalMapFlag = 0;
//	}
//
//	if (textureList.DisplacementMap.Width == 1 &&
//		textureList.DisplacementMap.Width == 1)
//	{
//		properites.MapFlags.DisplacementMapFlag = 0;
//	}
//
//	if (textureList.AlphaMap.Width == 1 &&
//		textureList.AlphaMap.Width == 1)
//	{
//		properites.MapFlags.AlphaMapFlag = 0;
//	}
//
//	if (textureList.CubeMap.Width == 1 &&
//		textureList.CubeMap.Width == 1)
//	{
//		properites.MapFlags.CubeMapFlag = 0;
//	}
//}

void Mesh::CreateUniformBuffers(VulkanRenderer& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    lightBuffer = VulkanUniformBuffer(renderer, sizeof(LightBufferObject));
    meshPropertiesBuffer = VulkanUniformBuffer(renderer, sizeof(MeshProperties));
}

void Mesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 10>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[9].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanRenderer& renderer, Texture tex2, Texture tex3, CubeMapTexture cubemap, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = texture.View;
    DiffuseMap.sampler = texture.Sampler;

    VkDescriptorImageInfo SpecularMap = {};
    SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SpecularMap.imageView = texture.View;
    SpecularMap.sampler = texture.Sampler;

    VkDescriptorImageInfo NormalMap = {};
    NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    NormalMap.imageView = tex2.View;
    NormalMap.sampler = tex2.Sampler;

    VkDescriptorImageInfo DisplacementMap = {};
    DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DisplacementMap.imageView = tex3.View;
    DisplacementMap.sampler = tex3.Sampler;

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = texture.View;
    AlphaMap.sampler = texture.Sampler;

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = texture.View;
    EmissionMap.sampler = texture.Sampler;

    VkDescriptorImageInfo SkyBoxMap = {};
    SkyBoxMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyBoxMap.imageView = cubemap.View;
    SkyBoxMap.sampler = cubemap.Sampler;

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = uniformBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(UniformBufferObject);

        VkDescriptorBufferInfo LightInfo = {};
        LightInfo.buffer = lightBuffer.GetUniformBuffer(i);
        LightInfo.offset = 0;
        LightInfo.range = sizeof(LightBufferObject);

        VkDescriptorBufferInfo meshPropertiesInfo = {};
        meshPropertiesInfo.buffer = meshPropertiesBuffer.GetUniformBuffer(i);
        meshPropertiesInfo.offset = 0;
        meshPropertiesInfo.range = sizeof(MeshProperties);

        std::vector<WriteDescriptorSetInfo> DescriptorList;

        WriteDescriptorSetInfo PositionDescriptor;
        PositionDescriptor.DstBinding = 0;
        PositionDescriptor.DstSet = DescriptorSets[i];
        PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        PositionDescriptor.DescriptorBufferInfo = PositionInfo;
        DescriptorList.emplace_back(PositionDescriptor);

        WriteDescriptorSetInfo DiffuseMapDescriptor;
        DiffuseMapDescriptor.DstBinding = 1;
        DiffuseMapDescriptor.DstSet = DescriptorSets[i];
        DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DiffuseMapDescriptor.DescriptorImageInfo = DiffuseMap;
        DescriptorList.emplace_back(DiffuseMapDescriptor);

        WriteDescriptorSetInfo SpecularMapDescriptor;
        SpecularMapDescriptor.DstBinding = 2;
        SpecularMapDescriptor.DstSet = DescriptorSets[i];
        SpecularMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        SpecularMapDescriptor.DescriptorImageInfo = SpecularMap;
        DescriptorList.emplace_back(SpecularMapDescriptor);

        WriteDescriptorSetInfo NormalMapDescriptor;
        NormalMapDescriptor.DstBinding = 3;
        NormalMapDescriptor.DstSet = DescriptorSets[i];
        NormalMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        NormalMapDescriptor.DescriptorImageInfo = NormalMap;
        DescriptorList.emplace_back(NormalMapDescriptor);

        WriteDescriptorSetInfo DisplacementMapDescriptor;
        DisplacementMapDescriptor.DstBinding = 4;
        DisplacementMapDescriptor.DstSet = DescriptorSets[i];
        DisplacementMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DisplacementMapDescriptor.DescriptorImageInfo = DisplacementMap;
        DescriptorList.emplace_back(DisplacementMapDescriptor);

        WriteDescriptorSetInfo AlphaMapDescriptor;
        AlphaMapDescriptor.DstBinding = 5;
        AlphaMapDescriptor.DstSet = DescriptorSets[i];
        AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
        DescriptorList.emplace_back(AlphaMapDescriptor);

        WriteDescriptorSetInfo EmissionMapDescriptor;
        EmissionMapDescriptor.DstBinding = 6;
        EmissionMapDescriptor.DstSet = DescriptorSets[i];
        EmissionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        EmissionMapDescriptor.DescriptorImageInfo = EmissionMap;
        DescriptorList.emplace_back(EmissionMapDescriptor);

        WriteDescriptorSetInfo SkyBoxDescriptor;
        SkyBoxDescriptor.DstBinding = 7;
        SkyBoxDescriptor.DstSet = DescriptorSets[i];
        SkyBoxDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        SkyBoxDescriptor.DescriptorImageInfo = SkyBoxMap;
        DescriptorList.emplace_back(SkyBoxDescriptor);

        WriteDescriptorSetInfo  MeshPropertiesDescriptor;
        MeshPropertiesDescriptor.DstBinding = 8;
        MeshPropertiesDescriptor.DstSet = DescriptorSets[i];
        MeshPropertiesDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MeshPropertiesDescriptor.DescriptorBufferInfo = meshPropertiesInfo;
        DescriptorList.emplace_back(MeshPropertiesDescriptor);

        WriteDescriptorSetInfo LightDescriptor;
        LightDescriptor.DstBinding = 9;
        LightDescriptor.DstSet = DescriptorSets[i];
        LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LightDescriptor.DescriptorBufferInfo = LightInfo;
        DescriptorList.emplace_back(LightDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh::Update(VulkanRenderer& renderer, Camera& camera, MeshProperties meshProp, LightBufferObject Lightbuffer)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera.GetViewMatrix();
    ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
    ubo.proj[1][1] *= -1;

    if (RotationAmount != 0 &&
        (MeshRotate.x != 0 ||
         MeshRotate.y != 0 ||
         MeshRotate.z != 0))
    {
        ubo.model = glm::rotate(ubo.model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    }

    UpdateUniformBuffer(renderer, ubo, meshProp, Lightbuffer);
}

void Mesh::UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, MeshProperties meshProp, LightBufferObject Lightbuffer)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
    lightBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&Lightbuffer));
    meshPropertiesBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&meshProp));
}

void Mesh::Destory(VulkanRenderer& renderer)
{
    uniformBuffer.Destroy(renderer);
    lightBuffer.Destroy(renderer);
    meshPropertiesBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}