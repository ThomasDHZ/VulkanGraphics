#include "Mesh2D.h"
#include "Texture2D.h"

Mesh2D::Mesh2D() : NewMesh()
{
}

Mesh2D::Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
}

Mesh2D::Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures) : NewMesh(renderer, vertexdata, indicesdata)
{
    CustomBuffer EmptyBuffer;
    EmptyBuffer.ByteSize = sizeof(Empty);

    Vertexdata = vertexdata;
    ExtendedMesProperitesBuffer = EmptyBuffer;

    CreateMaterialProperties();
    LoadTextures(renderer, textureManager, textures);
    LoadTiles(renderer, textureManager, textures);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
}

Mesh2D::Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer) : NewMesh(renderer, vertexdata, indicesdata, customBuffer)
{
    Vertexdata = vertexdata;
    ExtendedMesProperitesBuffer = customBuffer;

    LoadTextures(renderer, textureManager, textures);
    LoadTiles(renderer, textureManager, textures);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateMaterialProperties();
}

Mesh2D::~Mesh2D()
{
}

void Mesh2D::CreateDescriptorPool(RendererManager& renderer) {

    std::array<DescriptorPoolSizeInfo, 10>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[7].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[8].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    NewBaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh2D::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager)
{
    NewBaseMesh::CreateDescriptorSets(renderer, renderer.forwardRenderer.renderer2DPipeline->ShaderPipelineDescriptorLayout);

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = DiffuseMapID->GetTextureView();
    DiffuseMap.sampler = DiffuseMapID->GetTextureSampler();

    VkDescriptorImageInfo SpecularMap = {};
    SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SpecularMap.imageView = SpecularMapID->GetTextureView();
    SpecularMap.sampler = SpecularMapID->GetTextureSampler();

    VkDescriptorImageInfo NormalMap = {};
    NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    NormalMap.imageView = NormalMapID->GetTextureView();
    NormalMap.sampler = NormalMapID->GetTextureSampler();

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = AlphaMapID->GetTextureView();
    AlphaMap.sampler = AlphaMapID->GetTextureSampler();

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = DiffuseMapID->GetTextureView();
    EmissionMap.sampler = DiffuseMapID->GetTextureSampler();

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

        VkDescriptorBufferInfo emptyPropertiesInfo = {};
        emptyPropertiesInfo.buffer = ExtendedMesProperitesBuffer.customBuffer.GetUniformBuffer(i);
        emptyPropertiesInfo.offset = 0;
        emptyPropertiesInfo.range = ExtendedMesProperitesBuffer.ByteSize;

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

        WriteDescriptorSetInfo emptyDescriptor;
        emptyDescriptor.DstBinding = 8;
        emptyDescriptor.DstSet = DescriptorSets[i];
        emptyDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        emptyDescriptor.DescriptorBufferInfo = emptyPropertiesInfo;
        DescriptorList.emplace_back(emptyDescriptor);

        NewBaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}