
//-------
//	Pub
//-------

void ComputePipeline::Create(const char *shader)
{
	CreateComputePipe(shader);
	CreateCmdBuff();

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	vkCreateFence(init->device, &fenceInfo, nullptr, &fence);
	vkResetFences(init->device, 1, &fence);
}

void ComputePipeline::Dispatch(bool sync)
{
	VkSubmitInfo computeSubmitInfo = {};
	computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	computeSubmitInfo.commandBufferCount = 1;
	computeSubmitInfo.pCommandBuffers = cmdBuff;
	vkQueueSubmit(init->queue, 1, &computeSubmitInfo, (sync) ? fence : nullptr);

	if (sync)
	{
		vkWaitForFences(init->device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkResetFences(init->device, 1, &fence);
	}
}

void ComputePipeline::Destroy()
{
	vkDestroyPipeline(init->device, pipeline, nullptr);
}

//-------
//	Priv
//-------

void ComputePipeline::CreateComputePipe(const char *shader)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = dSetLayouts.size();
	pipelineLayoutInfo.pSetLayouts = dSetLayouts.data();
	vkCreatePipelineLayout(init->device, &pipelineLayoutInfo, nullptr, &pipeLayout);

	VkShaderModule computeShader = GetShaderModule(init->device, shader);
	VkPipelineShaderStageCreateInfo computeShaderStage = {};
	computeShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	computeShaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	computeShaderStage.module = computeShader;
	computeShaderStage.pName = "main";

	VkComputePipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineInfo.stage = computeShaderStage;
	pipelineInfo.layout = pipeLayout;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	vkCreateComputePipelines(init->device, nullptr, 1, &pipelineInfo, nullptr, &pipeline);
	vkDestroyShaderModule(init->device, computeShader, nullptr);
}

#define GROUP_THREAD_COUNT 16
void ComputePipeline::CreateCmdBuff()
{
	cmdBuff = new VkCommandBuffer;
	*cmdBuff = AllocateCommandBuffer(init->device, init->cmdPool);

	RecorderParams params;
	params.xGroupSize = init->screen.width / GROUP_THREAD_COUNT;
	params.yGroupSize = init->screen.height / GROUP_THREAD_COUNT;
	params.zGroupSize = 1;
	RecordCommandBuffer(*cmdBuff, pipeline, pipeLayout, dSets.data(), dSets.size(), VK_PIPELINE_BIND_POINT_COMPUTE, params);
}
