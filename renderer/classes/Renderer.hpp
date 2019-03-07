#ifndef RENDERER
    #define RENDERER

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include <Initter.hpp>

class Renderer
{
    private:
		Initter init;

    public:
        vk::Instance instance;
        vk::PhysicalDevice gpu;
        vk::Device device;
        vk::Queue queue;
        vk::CommandPool cmdPool;

		Renderer(Initter init)
		{
			instance = init.instance;
			gpu = init.gpu;
			device = init.device;
			queue = init.queue;
			cmdPool = init.cmdPool;

			this->init = init;
		}

		void AddUniformBuffer(std::string, uint32_t size)
		{

		}
};

#endif
