#ifndef DRAWER
    #define DRAWER

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

struct SwapchainBuffer
{
    VkImage image;
    VkImageView view;
};

class Drawer
{
    private:
		void CreateSwapchain();

		Initter *init;

		VkSemaphore imageAvailable;
		VkSemaphore renderingDone;
		VkFence fence;

		VkSwapchainKHR swapchain;
		VkCommandBuffer *cmdBuffers;

    public:
		uint32_t swapchainImageCount;
    	SwapchainBuffer *swapchainBuffers;

		Drawer(){}
		Drawer(Initter *init)
		{
			this->init = init;

			CreateSwapchain();

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			vkCreateSemaphore(init->device, &semaphoreInfo, nullptr, &imageAvailable);
			vkCreateSemaphore(init->device, &semaphoreInfo, nullptr, &renderingDone);

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			vkCreateFence(init->device, &fenceInfo, nullptr, &fence);
			vkResetFences(init->device, 1, &fence);
		}

		void SetPipeCmdBuffers(VkCommandBuffer *cmdBuffers)
		{
			this->cmdBuffers = cmdBuffers;
		}

		void DrawFrame();
};

#include <Drawer.cpp>

#endif
