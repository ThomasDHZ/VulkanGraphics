#include "Mesh.h"
#include "Texture2D.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit) : BaseMesh(renderBit)
{
    VertexSize = vertexdata.size();
    IndexSize = indicesdata.size();

    CreateVertexBuffer(renderer, vertexdata);

    if (IndexSize != 0)
    {
        CreateIndexBuffer(renderer, indicesdata);
    }

    LoadTextures(renderer, textureManager, textures);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager, cubemap, descriptorSetLayout);
    CreateMaterialProperties();
}

Mesh::~Mesh()
{
}

void Mesh::CreateMaterialProperties()
{
    properites.material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    properites.material.shininess = 32;
    properites.material.reflectivness = 0;
    properites.minLayers = 8.0f;
    properites.maxLayers = 32.0f;
    properites.heightScale = 0.1f;

    //properites.UseSkyBoxBit = 0;
}

void Mesh::LoadTextures(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures)
{
    if (!textures.DiffuseMap.empty())
    {
        DiffuseMapID = textureManager->LoadTexture(renderer, textures.DiffuseMap, VK_FORMAT_R8G8B8A8_SRGB);
        properites.UseDiffuseMapBit = 1;
    }
    if (!textures.SpecularMap.empty())
    {
        SpecularMapID = textureManager->LoadTexture(renderer, textures.SpecularMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseSpecularMapBit = 1;
    }
    if (!textures.NormalMap.empty())
    {
        NormalMapID = textureManager->LoadTexture(renderer, textures.NormalMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseNormalMapBit = 1;
    }
    if (!textures.DepthMap.empty())
    {
        DepthMapID = textureManager->LoadTexture(renderer, textures.DepthMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseDepthMapBit = 1;
    }
    if (!textures.AlphaMap.empty())
    {
        AlphaMapID = textureManager->LoadTexture(renderer, textures.AlphaMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseAlphaMapBit = 1;
    }
    if (!textures.EmissionMap.empty())
    {
        EmissionMapID = textureManager->LoadTexture(renderer, textures.EmissionMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseEmissionMapBit = 1;
    }
    if (!textures.ReflectionMap.empty())
    {
        ReflectionMapID = textureManager->LoadTexture(renderer, textures.ReflectionMap, VK_FORMAT_R8G8B8A8_UNORM);
        properites.UseReflectionMapBit = 1;
    }
}

void Mesh::CreateUniformBuffers(VulkanRenderer& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    lightBuffer = VulkanUniformBuffer(renderer, sizeof(LightBufferObject));
    meshPropertiesBuffer = VulkanUniformBuffer(renderer, sizeof(MeshProperties));
}

void Mesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 11>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[9].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[10].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, int cubemap, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = textureManager->GetTextureByID(DiffuseMapID).View;
    DiffuseMap.sampler = textureManager->GetTextureByID(DiffuseMapID).Sampler;

    VkDescriptorImageInfo SpecularMap = {};
    SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SpecularMap.imageView = textureManager->GetTextureByID(SpecularMapID).View;
    SpecularMap.sampler = textureManager->GetTextureByID(SpecularMapID).Sampler;

    VkDescriptorImageInfo NormalMap = {};
    NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    NormalMap.imageView = textureManager->GetTextureByID(NormalMapID).View;
    NormalMap.sampler = textureManager->GetTextureByID(NormalMapID).Sampler;

    VkDescriptorImageInfo DisplacementMap = {};
    DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DisplacementMap.imageView = textureManager->GetTextureByID(DepthMapID).View;
    DisplacementMap.sampler = textureManager->GetTextureByID(DepthMapID).Sampler;

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = textureManager->GetTextureByID(DiffuseMapID).View;
    AlphaMap.sampler = textureManager->GetTextureByID(DiffuseMapID).Sampler;

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = textureManager->GetTextureByID(DiffuseMapID).View;
    EmissionMap.sampler = textureManager->GetTextureByID(DiffuseMapID).Sampler;

    VkDescriptorImageInfo ReflectionMap = {};
    ReflectionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ReflectionMap.imageView = textureManager->GetTextureByID(ReflectionMapID).View;
    ReflectionMap.sampler = textureManager->GetTextureByID(ReflectionMapID).Sampler;

    VkDescriptorImageInfo SkyBoxMap = {};
    SkyBoxMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyBoxMap.imageView = textureManager->GetTextureByID(cubemap).View;
    SkyBoxMap.sampler = textureManager->GetTextureByID(cubemap).Sampler;

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

        WriteDescriptorSetInfo ReflectionMapDescriptor;
        ReflectionMapDescriptor.DstBinding = 7;
        ReflectionMapDescriptor.DstSet = DescriptorSets[i];
        ReflectionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        ReflectionMapDescriptor.DescriptorImageInfo = ReflectionMap;
        DescriptorList.emplace_back(ReflectionMapDescriptor);

        WriteDescriptorSetInfo SkyBoxDescriptor;
        SkyBoxDescriptor.DstBinding = 8;
        SkyBoxDescriptor.DstSet = DescriptorSets[i];
        SkyBoxDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        SkyBoxDescriptor.DescriptorImageInfo = SkyBoxMap;
        DescriptorList.emplace_back(SkyBoxDescriptor);

        WriteDescriptorSetInfo  MeshPropertiesDescriptor;
        MeshPropertiesDescriptor.DstBinding = 9;
        MeshPropertiesDescriptor.DstSet = DescriptorSets[i];
        MeshPropertiesDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MeshPropertiesDescriptor.DescriptorBufferInfo = meshPropertiesInfo;
        DescriptorList.emplace_back(MeshPropertiesDescriptor);

        WriteDescriptorSetInfo LightDescriptor;
        LightDescriptor.DstBinding = 10;
        LightDescriptor.DstSet = DescriptorSets[i];
        LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LightDescriptor.DescriptorBufferInfo = LightInfo;
        DescriptorList.emplace_back(LightDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh::Update(VulkanRenderer& renderer, Camera& camera, LightBufferObject Lightbuffer)
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

    UpdateUniformBuffer(renderer, ubo, Lightbuffer);
}

void Mesh::UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
    lightBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&Lightbuffer));
    meshPropertiesBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&properites));
}

void Mesh::Destory(VulkanRenderer& renderer)
{
    uniformBuffer.Destroy(renderer);
    lightBuffer.Destroy(renderer);
    meshPropertiesBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}