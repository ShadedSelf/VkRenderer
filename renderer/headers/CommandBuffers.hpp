#ifndef COMMAND_BUFFERS
	#define COMMAND_BUFFERS

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

struct RecorderParams
{
	//	Compute
	uint32_t xGroupSize;
	uint32_t yGroupSize;
	uint32_t zGroupSize;

	//	Graphics
	VkRenderPass renderPass;
	VkFramebuffer frameBuffer;
	VkExtent2D extent;
};

vk::CommandBuffer AllocateCommandBuffer(vk::Device device, vk::CommandPool cmdPool);

VkCommandBuffer AllocateCommandBuffer(VkDevice device, VkCommandPool cmdPool);
VkCommandBuffer BeginTemporaryCmdBuffer(VkDevice device, VkCommandPool cmdPool);
void EndTemporaryCmdBuffer(VkDevice device, VkCommandPool cmdPool, VkQueue queue, VkCommandBuffer commandBuffer);
void RecordCommandBuffer(VkCommandBuffer cmdBuff, VkPipeline pipe, VkPipelineLayout pipeLayout, VkDescriptorSet *dSets,
							uint32_t dSetsCount, VkPipelineBindPoint bindPoint, RecorderParams params);

#include <CommandBuffers.cpp>

#endif
