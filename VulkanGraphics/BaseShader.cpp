#include "BaseShader.h"
#include <array>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>

BaseShader::BaseShader()
{
}

BaseShader::BaseShader(VulkanDevice deviceInfo)
{
	DeviceInfo = deviceInfo;
}

BaseShader::~BaseShader()
{
}

std::vector<char> BaseShader::ReadShaderFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule BaseShader::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(DeviceInfo.Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

void BaseShader::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo)
{
	std::vector<VkDescriptorSetLayoutBinding> LayoutBindingList = {};

	for (auto Binding : LayoutBindingInfo)
	{
		VkDescriptorSetLayoutBinding LayoutBinding = {};
		LayoutBinding.binding = Binding.Binding;
		LayoutBinding.descriptorCount = 1;
		LayoutBinding.descriptorType = Binding.DescriptorType;
		LayoutBinding.pImmutableSamplers = nullptr;
		LayoutBinding.stageFlags = Binding.StageFlags;

		LayoutBindingList.emplace_back(LayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindingList.size());
	layoutInfo.pBindings = LayoutBindingList.data();

	if (vkCreateDescriptorSetLayout(DeviceInfo.Device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void BaseShader::CreateShaderPipeLine()
{
}

void BaseShader::CreateUniformBuffers()
{

}

void BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
{
	std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

	for (auto DescriptorPool : DescriptorPoolInfo)
	{
		VkDescriptorPoolSize DescriptorPoolBinding = {};
		DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
		DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(DeviceInfo.SwapChainSize);
		DescriptorPoolList.emplace_back(DescriptorPoolBinding);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
	poolInfo.pPoolSizes = DescriptorPoolList.data();
	poolInfo.maxSets = static_cast<uint32_t>(DeviceInfo.SwapChainSize);

	if (vkCreateDescriptorPool(DeviceInfo.Device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void BaseShader::CreateDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(DeviceInfo.SwapChainSize, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(DeviceInfo.SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(DeviceInfo.SwapChainSize);
	if (vkAllocateDescriptorSets(DeviceInfo.Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void BaseShader::CreateDescriptorSets(VkDescriptorSetLayout layout)
{
	std::vector<VkDescriptorSetLayout> layouts(DeviceInfo.SwapChainSize, layout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(DeviceInfo.SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(DeviceInfo.SwapChainSize);
	if (vkAllocateDescriptorSets(DeviceInfo.Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList)
{
	std::vector<VkWriteDescriptorSet>  WriteDescriptorInfo = {};

	for (int x = 0; x < descriptorWritesList.size(); x++)
	{
		VkWriteDescriptorSet DescriptorSet = {};
		DescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		DescriptorSet.dstSet = descriptorWritesList[x].DstSet;
		DescriptorSet.dstBinding = descriptorWritesList[x].DstBinding;
		DescriptorSet.dstArrayElement = 0;
		DescriptorSet.descriptorType = descriptorWritesList[x].DescriptorType;
		DescriptorSet.descriptorCount = 1;
		if (descriptorWritesList[x].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			DescriptorSet.pBufferInfo = &descriptorWritesList[x].DescriptorBufferInfo;
		}
		else
		{
			DescriptorSet.pImageInfo = &descriptorWritesList[x].DescriptorImageInfo;
		}
		WriteDescriptorInfo.emplace_back(DescriptorSet);
	}

	vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
}

void BaseShader::UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
{
	void* UniformData;
	vkMapMemory(DeviceInfo.Device, UniformBufferMemory, 0, UniformSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, UniformSize);
	vkUnmapMemory(DeviceInfo.Device, UniformBufferMemory);
}

void BaseShader::Destory()
{
	//vkDestroyDescriptorSetLayout(DeviceInfo.Device, descriptorSetLayout, nullptr);
}

void BaseShader::DestorySwapChain()
{
	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		if (uniformBuffers.size() > 0)
		{
			vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[x], nullptr);
			vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[x], nullptr);
		}

		if (LightFragmentUniformBuffers.size() > 0)
		{
			vkDestroyBuffer(DeviceInfo.Device, LightFragmentUniformBuffers[x], nullptr);
			vkFreeMemory(DeviceInfo.Device, LightFragmentUniformBuffersMemory[x], nullptr);
		}
	}

	vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);
	vkDestroyPipeline(DeviceInfo.Device, ShaderPipeline, nullptr);
 //   vkDestroyPipelineLayout(DeviceInfo.Device, ShaderPipelineLayout, nullptr);
}

