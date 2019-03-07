#ifndef COMPUTE_PIPELINE
	#define COMPUTE_PIPELINE

#include <vulkan/vulkan.h>

#include <Common.hpp>
#include <Shaders.hpp>
#include <CommandBuffers.hpp>

class ComputePipeline
{
	private:
		Initter *init;
		VkFence fence;

		VkPipeline pipeline;
		VkPipelineLayout pipeLayout;
		VkCommandBuffer *cmdBuff;

		std::vector<VkDescriptorSet> dSets;
		std::vector<VkDescriptorSetLayout> dSetLayouts;

		void CreateComputePipe(const char *shader);
		void CreateCmdBuff();

	public:

		ComputePipeline(Initter *init)
		{
			this->init = init;
		}
		void BindBuffers(BindableBuffers *data)
		{
			data->CreateDescriptors(&dSets, &dSetLayouts, VK_SHADER_STAGE_COMPUTE_BIT);
		}
		void Create(const char *shader);
		void Dispatch(bool sync);
		void Destroy();
};

#include <ComputePipeline.cpp>

#endif
