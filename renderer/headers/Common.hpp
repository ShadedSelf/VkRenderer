#ifndef COMMON
	#define COMMON

#include <vulkan/vulkan.h>

struct Globals
{
	VkDevice device;
	VkCommandPool cmdPool;
	VkPhysicalDevice gpu;
	VkQueue queue;

	uint32_t width;
	uint32_t height;
};

#define GROUP_THREAD_COUNT 16

#endif
