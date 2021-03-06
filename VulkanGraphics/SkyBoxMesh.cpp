#include "SkyBoxMesh.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBoxMesh::SkyBoxMesh() : Mesh()
{
}

SkyBoxMesh::SkyBoxMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, MeshTextures textures) : Mesh(renderer, SkyBoxVertices)
{
	LoadTextures(renderer, textureManager, textures);
	CreateUniformBuffers(renderer);
	CreateDescriptorPool(renderer);
	CreateDescriptorSets(renderer, textureManager);
	CreateDrawMessage(renderer, 1, renderer.forwardRenderer.skyboxPipeline);
	CreateDrawMessage(renderer, 2, renderer.textureRenderer.skyboxPipeline);
}

SkyBoxMesh::~SkyBoxMesh()
{
}

void SkyBoxMesh::CreateUniformBuffers(RendererManager& renderer)
{
	uniformBuffer = VulkanUniformBuffer(renderer, sizeof(VertexMatrixObject));
}

void SkyBoxMesh::CreateDescriptorPool(RendererManager& renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBoxMesh::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager)
{
	BaseMesh::CreateDescriptorSets(renderer, renderer.forwardRenderer.skyboxPipeline->ShaderPipelineDescriptorLayout);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = SkyBoxTexture->GetTextureView();
	imageInfo.sampler = SkyBoxTexture->GetTextureSampler();

	for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffer.GetUniformBuffer(i);
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(VertexMatrixObject);

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(renderer.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBoxMesh::UpdateUniformBuffer(RendererManager& renderer, std::shared_ptr<Camera> camera)
{
	VertexMatrixObject ubo{};
	ubo.view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	ubo.proj = glm::perspective(glm::radians(camera->GetZoom()), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
	ubo.proj[1][1] *= -1;

	uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
}

void SkyBoxMesh::Destory(RendererManager& renderer)
{
	uniformBuffer.Destroy(renderer);
	BaseMesh::Destory(renderer);
}
