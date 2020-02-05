#include "BaseShader.h"
#include <array>

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

void BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWrites)
{
	std::vector<VkWriteDescriptorSet> DescriptorPoolList = {};

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		
			VkWriteDescriptorSet DescriptorSet;
			DescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			DescriptorSet.dstSet = descriptorSets[i];
			DescriptorSet.dstBinding = descriptorWrites[i].DstBinding;
			DescriptorSet.dstArrayElement = 0;
			DescriptorSet.descriptorType = descriptorWrites[i].DescriptorType;
			DescriptorSet.descriptorCount = 1;

			if (descriptorWrites[i].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{
				DescriptorSet.pBufferInfo = &descriptorWrites[i].DescriptorBufferInfo;
			}
			else
			{
				DescriptorSet.pImageInfo = &descriptorWrites[i].DescriptorImageInfo;
			}

			DescriptorPoolList.emplace_back(DescriptorSet);
		

		vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(DescriptorPoolList.size()), DescriptorPoolList.data(), 0, nullptr);
	}
}

void BaseShader::Destory()
{
	vkDestroyDescriptorSetLayout(DeviceInfo.Device, descriptorSetLayout, nullptr);
}

void BaseShader::DestorySwapChain()
{
	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[x], nullptr);

		vkDestroyBuffer(DeviceInfo.Device, FragmentUniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, FragmentUniformBuffersMemory[x], nullptr);
	}

	vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);
}

