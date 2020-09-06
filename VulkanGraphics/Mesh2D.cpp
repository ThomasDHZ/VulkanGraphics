#include "Mesh2D.h"
#include "Texture2D.h"

Mesh2D::Mesh2D() : NewBaseMesh()
{
}

Mesh2D::Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int renderBit) : NewBaseMesh(renderBit)
{
    Vertexdata = vertexdata;

    LoadTextures(renderer, textureManager, textures);
    LoadTiles(renderer, textureManager, textures);
    MeshVertex = VertexBuffer(renderer, Vertexdata);
    MeshIndices = IndicesBuffer(renderer, indicesdata);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateMaterialProperties();
}

Mesh2D::Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int renderBit, Texture& texture) : NewBaseMesh(renderBit)
{
    Vertexdata = vertexdata;

    LoadTextures(renderer, textureManager, textures);
    LoadTiles(renderer, textureManager, textures);
    MeshVertex = VertexBuffer(renderer, Vertexdata);
    MeshIndices = IndicesBuffer(renderer, indicesdata);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager, texture);
    CreateMaterialProperties();
}

Mesh2D::~Mesh2D()
{
}

void Mesh2D::CreateMaterialProperties()
{
    properites.material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    properites.material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    properites.material.shininess = 32;
    properites.material.reflectivness = 0;
    properites.minLayers = 8.0f;
    properites.maxLayers = 32.0f;
    properites.heightScale = 0.1f;
}

void Mesh2D::LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures)
{

    if (!textures.DiffuseMap.empty())
    {
        properites.UseDiffuseMapBit = 1;
    }
    if (!textures.SpecularMap.empty())
    {
        properites.UseSpecularMapBit = 1;
    }
    if (!textures.NormalMap.empty())
    {
        properites.UseNormalMapBit = 1;
    }
    if (!textures.AlphaMap.empty())
    {
        properites.UseAlphaMapBit = 1;
    }
    if (!textures.EmissionMap.empty())
    {
        properites.UseEmissionMapBit = 1;
    }

}

void Mesh2D::CreateUniformBuffers(RendererManager& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    lightBuffer = VulkanUniformBuffer(renderer, sizeof(LightBufferObject));
    meshPropertiesBuffer = VulkanUniformBuffer(renderer, sizeof(MeshProperties));
}

void Mesh2D::CreateDescriptorPool(RendererManager& renderer) {

    std::array<DescriptorPoolSizeInfo, 9>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    NewBaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh2D::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager)
{
    NewBaseMesh::CreateDescriptorSets(renderer, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout);

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

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = textureManager->GetTextureByID(AlphaMapID).View;
    AlphaMap.sampler = textureManager->GetTextureByID(AlphaMapID).Sampler;

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = textureManager->GetTextureByID(DiffuseMapID).View;
    EmissionMap.sampler = textureManager->GetTextureByID(DiffuseMapID).Sampler;

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

        WriteDescriptorSetInfo AlphaMapDescriptor;
        AlphaMapDescriptor.DstBinding = 4;
        AlphaMapDescriptor.DstSet = DescriptorSets[i];
        AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
        DescriptorList.emplace_back(AlphaMapDescriptor);

        WriteDescriptorSetInfo EmissionMapDescriptor;
        EmissionMapDescriptor.DstBinding = 5;
        EmissionMapDescriptor.DstSet = DescriptorSets[i];
        EmissionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        EmissionMapDescriptor.DescriptorImageInfo = EmissionMap;
        DescriptorList.emplace_back(EmissionMapDescriptor);

        WriteDescriptorSetInfo  MeshPropertiesDescriptor;
        MeshPropertiesDescriptor.DstBinding = 6;
        MeshPropertiesDescriptor.DstSet = DescriptorSets[i];
        MeshPropertiesDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MeshPropertiesDescriptor.DescriptorBufferInfo = meshPropertiesInfo;
        DescriptorList.emplace_back(MeshPropertiesDescriptor);

        WriteDescriptorSetInfo LightDescriptor;
        LightDescriptor.DstBinding = 7;
        LightDescriptor.DstSet = DescriptorSets[i];
        LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LightDescriptor.DescriptorBufferInfo = LightInfo;
        DescriptorList.emplace_back(LightDescriptor);

        NewBaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh2D::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, Texture& texture)
{
    NewBaseMesh::CreateDescriptorSets(renderer, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = texture.View;;
    DiffuseMap.sampler = texture.Sampler;

    VkDescriptorImageInfo SpecularMap = {};
    SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SpecularMap.imageView = textureManager->GetTextureByID(SpecularMapID).View;
    SpecularMap.sampler = textureManager->GetTextureByID(SpecularMapID).Sampler;

    VkDescriptorImageInfo NormalMap = {};
    NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    NormalMap.imageView = textureManager->GetTextureByID(NormalMapID).View;
    NormalMap.sampler = textureManager->GetTextureByID(NormalMapID).Sampler;

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = textureManager->GetTextureByID(AlphaMapID).View;
    AlphaMap.sampler = textureManager->GetTextureByID(AlphaMapID).Sampler;

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = textureManager->GetTextureByID(DiffuseMapID).View;
    EmissionMap.sampler = textureManager->GetTextureByID(DiffuseMapID).Sampler;

    VkDescriptorImageInfo ReflectDiffuseMap = {};
    ReflectDiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ReflectDiffuseMap.imageView = textureManager->GetTextureByID(NormalMapID).View;
    ReflectDiffuseMap.sampler = textureManager->GetTextureByID(NormalMapID).Sampler;

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

        WriteDescriptorSetInfo AlphaMapDescriptor;
        AlphaMapDescriptor.DstBinding = 4;
        AlphaMapDescriptor.DstSet = DescriptorSets[i];
        AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
        DescriptorList.emplace_back(AlphaMapDescriptor);

        WriteDescriptorSetInfo EmissionMapDescriptor;
        EmissionMapDescriptor.DstBinding = 5;
        EmissionMapDescriptor.DstSet = DescriptorSets[i];
        EmissionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        EmissionMapDescriptor.DescriptorImageInfo = EmissionMap;
        DescriptorList.emplace_back(EmissionMapDescriptor);

        WriteDescriptorSetInfo  MeshPropertiesDescriptor;
        MeshPropertiesDescriptor.DstBinding = 6;
        MeshPropertiesDescriptor.DstSet = DescriptorSets[i];
        MeshPropertiesDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MeshPropertiesDescriptor.DescriptorBufferInfo = meshPropertiesInfo;
        DescriptorList.emplace_back(MeshPropertiesDescriptor);

        WriteDescriptorSetInfo LightDescriptor;
        LightDescriptor.DstBinding = 7;
        LightDescriptor.DstSet = DescriptorSets[i];
        LightDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        LightDescriptor.DescriptorBufferInfo = LightInfo;
        DescriptorList.emplace_back(LightDescriptor);

        NewBaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh2D::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera.GetViewMatrix();
    ubo.proj = camera.GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    properites.timer = glfwGetTime();
    UpdateUniformBuffer(renderer, ubo, Lightbuffer);
}

void Mesh2D::UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
    lightBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&Lightbuffer));
    meshPropertiesBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&properites));
}

void Mesh2D::Destory(RendererManager& renderer)
{
    uniformBuffer.Destroy(renderer);
    lightBuffer.Destroy(renderer);
    meshPropertiesBuffer.Destroy(renderer);
    NewBaseMesh::Destory(renderer);
}