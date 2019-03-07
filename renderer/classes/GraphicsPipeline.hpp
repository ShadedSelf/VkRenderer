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
