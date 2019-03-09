#ifndef BINDABLE_OBJECT
    #define BINDABLE_OBJECT

#include <vulkan/vulkan.h>

enum BufferType
{
	UniformBuffer  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	StorageBuffer  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	StorageTexture = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
	SamplerTexture = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
};

class BindableObject 
{ 
    public:
        bool buffer;
        BufferType dType;
        std::string name;
		uint32_t binding;

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice gpu);
};

uint32_t BindableObject::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice gpu)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
	return 0;
}

#endif
