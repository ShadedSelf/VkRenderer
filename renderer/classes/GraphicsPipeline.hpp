#ifndef GRAPHICS_PIPELINE
    #define GRAPHICS_PIPELINE

#include <vulkan/vulkan.h>

#include <Shaders.hpp>
#include <CommandBuffers.hpp>

class GraphicsPipeline
{
	private:
		void CreateRenderPass();
		void CreateFrameBuffers(SwapchainBuffer *swapchainBuffers);
		void CreateCommandBuffers();
		void CreateGraphicsPipe(const char *vertexShaderPath, const char *fragmentShaderPath);

		std::vector<VkDescriptorSet> dSets;
		std::vector<VkDescriptorSetLayout> dSetLayouts;

	public:
		Initter *init;

		VkPipelineLayout pipeLayout;
		VkPipeline pipe;
		VkCommandBuffer *cmdBuffers;

		VkRenderPass renderPass;
		VkFramebuffer *frameBuffers;
		uint32_t swapchainImageCount;

	GraphicsPipeline(Initter *init)
	{
		this->init = init;
	}
	~GraphicsPipeline()
	{
		vkDestroyRenderPass(init->device, renderPass, nullptr);
		for	(uint32_t i = 0; i < swapchainImageCount; i++)
			vkDestroyFramebuffer(init->device, frameBuffers[i], nullptr);
		delete[] frameBuffers;
		vkDestroyPipelineLayout(init->device, pipeLayout, nullptr);
		vkDestroyPipeline(init->device, pipe, nullptr);
		FlushDescriptors();			
	}
	void FlushDescriptors()
	{
		for(size_t i = 0; i < dSetLayouts.size(); i++)
			vkDestroyDescriptorSetLayout(init->device, dSetLayouts[i], nullptr);
		dSetLayouts.clear();
		dSets.clear();
	}
	void BindBuffers(BindableBuffers *data)
	{
		data->CreateDescriptors(&dSets, &dSetLayouts, VK_SHADER_STAGE_ALL_GRAPHICS);
	}

	void Create(uint32_t swapchainImageCount, SwapchainBuffer *swapchainBuffers, const char *vertexShaderPath, const char *fragmentShaderPath)
	{
		this->swapchainImageCount = swapchainImageCount;

		CreateRenderPass();
		CreateFrameBuffers(swapchainBuffers);
		CreateGraphicsPipe(vertexShaderPath, fragmentShaderPath);
		CreateCommandBuffers();
	}
};

#include <GraphicsPipeline.cpp>

#endif
