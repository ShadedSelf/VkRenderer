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
		~ComputePipeline()
		{
			vkDestroyFence(init->device, fence, nullptr);
			vkDestroyPipelineLayout(init->device, pipeLayout, nullptr);
			vkDestroyPipeline(init->device, pipeline, nullptr);
			FlushDescriptors();			
		}
		void FlushDescriptors();
		void BindBuffers(BindableBuffers *data);
		void Create(const char *shader);
		void Dispatch(bool sync);
		void Destroy();
};

#include <ComputePipeline.cpp>

#endif
