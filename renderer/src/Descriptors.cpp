#include <Descriptors.hpp>
#include <stdlib.h>
#include <cstring>

VkDescriptorSetLayout CreateDescriptorSetLayout(VkDevice device, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t *bindings, uint32_t bindingCount)
{
	VkDescriptorSetLayoutBinding *layoutBindings = new VkDescriptorSetLayoutBinding[bindingCount];
	for (uint32_t i = 0; i < bindingCount; ++i)
	{
	    layoutBindings[i].binding = bindings[i];
		layoutBindings[i].descriptorType = descriptorType;
		layoutBindings[i].descriptorCount = 1;
		layoutBindings[i].stageFlags = stageFlags;
		layoutBindings[i].pImmutableSamplers = nullptr;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = bindingCount;
	layoutInfo.pBindings = layoutBindings;

	VkDescriptorSetLayout dSetLayout;
	vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &dSetLayout);
	delete[] layoutBindings;
	return dSetLayout;
}

void UpdateDescriptors(VkDevice device, VkDescriptorSet *dSets, uint32_t dSetCount, uint32_t paramCount, SetCreateParams *params)
{
	VkWriteDescriptorSet *dWrite = (VkWriteDescriptorSet*)calloc(paramCount, sizeof(VkWriteDescriptorSet));
	for (uint32_t i = 0; i < paramCount; ++i)
	{
		dWrite[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		dWrite[i].dstSet = dSets[params[i].dSetIndex];
		dWrite[i].dstBinding = params[i].dstBinding;
		dWrite[i].descriptorType = params[i].dType;
		dWrite[i].descriptorCount = 1;

		if (params[i].dType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || params[i].dType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
			dWrite[i].pBufferInfo = &params[i].bufferInfo;
		else
			dWrite[i].pImageInfo = &params[i].imageInfo;
	}
	vkUpdateDescriptorSets(device, paramCount, dWrite, 0, nullptr);
	free(dWrite);
}

VkDescriptorSet *CreateDesciptorSets(VkDevice device, VkDescriptorSetLayout *dSetLayouts, uint32_t dSetCount, VkDescriptorPool dPool, uint32_t paramCount, SetCreateParams *params)
{
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = dPool;
	allocInfo.descriptorSetCount = dSetCount;
	allocInfo.pSetLayouts = dSetLayouts;

	VkDescriptorSet *dSets = new VkDescriptorSet[dSetCount];
	vkAllocateDescriptorSets(device, &allocInfo, dSets);
	UpdateDescriptors(device, dSets, dSetCount, paramCount, params);
	return dSets;
}
