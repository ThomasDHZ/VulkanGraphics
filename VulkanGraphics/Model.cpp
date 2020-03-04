#include "Model.h"

Model::Model() : Mesh()
{
}

Model::Model(MainPipeline pipeline, VulkanDevice deviceInfo, std::vector<Texture2D> TextureSet, std::vector<Vertex> vertices, std::vector<uint16_t> indices) : Mesh(deviceInfo)
{
	VertexSize = vertices.size();
	IndiceSize = indices.size();

	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(pipeline, TextureSet);
	CreateVertexBuffer(vertices);
	CreateIndiceBuffer(indices);
}

Model::Model(VulkanDevice deviceInfo, const std::string& FilePath) : Mesh(deviceInfo)
{
	ModelLoader(FilePath);
}

Model::~Model()
{
}

void Model::LoadVertices(aiMesh* mesh)
{
	for (int x = 0; x < mesh->mNumVertices; x++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3{ mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z };
		vertex.Normal = glm::vec3{ mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z };
		vertex.Tangant = glm::vec3{ mesh->mTangents[x].x, mesh->mTangents[x].y, mesh->mTangents[x].z };
		vertex.BiTangant = glm::vec3{ mesh->mBitangents[x].x, mesh->mBitangents[x].y, mesh->mBitangents[x].z };

		if (mesh->mTextureCoords[0])
		{
			vertex.TexureCoord = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
		}
		else
		{
			vertex.TexureCoord = glm::vec2{ 0.0f, 0.0f };
		}

		Vertices.emplace_back(vertex);
	}
}

void Model::LoadIndices(aiMesh* mesh)
{
	for (int x = 0; x < mesh->mNumVertices; x++)
	{
		aiFace Faces = mesh->mFaces[x];
		for (int y = 0; y < Faces.mNumIndices; y++)
		{
			Indices.emplace_back(Faces.mIndices[y]);
		}
	}
}

void Model::LoadTextures(const std::string& FilePath, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto directory = FilePath.substr(0, FilePath.find_last_of('/')) + '/';
	//for (int x = 0; x <= aiTextureType_UNKNOWN; x++)
	//{
		for (unsigned int y = 0; y < material->GetTextureCount(aiTextureType_DIFFUSE); y++)
		{


			aiString TextureLocation;
			material->GetTexture(aiTextureType_DIFFUSE, y, &TextureLocation);
			TexureList.emplace_back(Texture2D(DeviceInfo, directory + TextureLocation.C_Str()));
		}
	//}
}

void Model::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, vertexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void Model::CreateIndiceBuffer(std::vector<uint16_t> indices)
{
	if (IndiceSize != 0)
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, indexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

		vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
	}
}

void Model::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject2);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Model::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 3>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	Mesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Model::CreateDescriptorSets(MainPipeline pipeline, std::vector<Texture2D> TextureSet)
{
	Mesh::CreateDescriptorSets(pipeline.ShaderPipelineDescriptorLayout);

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureSet[0].textureImageView;
	DiffuseMap.sampler = TextureSet[0].textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureSet[1].textureImageView;
	SpecularMap.sampler = TextureSet[1].textureSampler;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		std::array<WriteDescriptorSetInfo, 3>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		Mesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Model::ModelLoader(const std::string& FilePath)
{
	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "Error loading model: " << ModelImporter.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(FilePath, Scene->mRootNode, Scene);
}

void Model::ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene)
{

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadVertices(mesh);
		//LoadIndices(mesh);
		LoadTextures(FilePath, mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(FilePath, node->mChildren[i], scene);
	}

}

void Model::UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage)
{
	Mesh::UpdateUniformBuffer(uniformBuffersMemory[currentImage], static_cast<void*>(&ubo2), sizeof(ubo2));
}

void Model::RecreateSwapChainStage(MainPipeline pipeline, VkExtent2D swapChainExtent, VkRenderPass renderPass, std::vector<Texture2D> TextureSet)
{
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(pipeline, TextureSet);
}

void Model::Destroy()
{
	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[x], nullptr);
	}

	Mesh::Destory();
}