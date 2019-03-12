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
		void CreateSyncObjects();

		Initter *init;

		VkSemaphore imageAvailable;
		VkSemaphore renderingDone;
		VkFence fence;

		VkSwapchainKHR swapchain;
		VkCommandBuffer *cmdBuffers;

    public:
		uint32_t swapchainImageCount;
    	SwapchainBuffer *swapchainBuffers;

		Drawer(Initter *init)
		{
			this->init = init;

			CreateSwapchain();
			CreateSyncObjects();
		}
		~Drawer()
		{
			vkDestroySemaphore(init->device, imageAvailable, nullptr);
			vkDestroySemaphore(init->device, renderingDone, nullptr);
			vkDestroyFence(init->device, fence, nullptr);
			vkDestroySwapchainKHR(init->device, swapchain, nullptr);
		}

		void SetPipeCmdBuffers(VkCommandBuffer *cmdBuffers)
		{
			this->cmdBuffers = cmdBuffers;
		}

		void DrawFrame();
};

#include <Drawer.cpp>

#endif
