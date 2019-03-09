#include <CommandBuffers.hpp>

VkCommandBuffer AllocateCommandBuffer(VkDevice device, VkCommandPool cmdPool)
{
	VkCommandBuffer cmdBuff;
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = cmdPool;
	allocInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(device, &allocInfo, &cmdBuff);

	return cmdBuff;
}

VkCommandBuffer BeginTemporaryCmdBuffer(VkDevice device, VkCommandPool cmdPool)
{
	VkCommandBuffer cmdBuff = AllocateCommandBuffer(device, cmdPool);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmdBuff, &beginInfo);

	return cmdBuff;
}

void EndTemporaryCmdBuffer(VkDevice device, VkCommandPool cmdPool, VkQueue queue, VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);
	vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
}

//	Probably better to have separate for graphics and compute
void RecordCommandBuffer(VkCommandBuffer cmdBuff,
						VkPipeline pipe,
						VkPipelineLayout pipeLayout,
						VkDescriptorSet *dSets,
						uint32_t dSetsCount,
						VkPipelineBindPoint bindPoint,
						RecorderParams params)
{
	VkRenderPassBeginInfo renderPassInfo = {};
	if (bindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS)
	{
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = params.renderPass;
		renderPassInfo.framebuffer = params.frameBuffer;
		renderPassInfo.renderArea.offset.x = 0;
		renderPassInfo.renderArea.offset.y = 0;
		renderPassInfo.renderArea.extent = params.extent;

		VkClearValue clearColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
	}

	VkCommandBufferBeginInfo cmdBufInfo = {};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //	Not needed?

	vkBeginCommandBuffer(cmdBuff, &cmdBufInfo);
	if (bindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS)
		vkCmdBeginRenderPass(cmdBuff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(cmdBuff, bindPoint, pipe);
			vkCmdBindDescriptorSets(cmdBuff, bindPoint, pipeLayout, 0, dSetsCount, dSets, 0, 0);
		if (bindPoint == VK_PIPELINE_BIND_POINT_COMPUTE)
			vkCmdDispatch(cmdBuff, params.xGroupSize, params.yGroupSize, params.zGroupSize);
		else
			vkCmdDraw(cmdBuff, 3, 1, 0, 0);

	if (bindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS)
		vkCmdEndRenderPass(cmdBuff);
	vkEndCommandBuffer(cmdBuff);
}
