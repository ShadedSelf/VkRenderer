#ifndef SHADERS
	#define SHADERS

#include <vulkan/vulkan.hpp>

#include <Shaders.cpp>

VkShaderModule GetShaderModule(VkDevice device, const char *path);

#endif
