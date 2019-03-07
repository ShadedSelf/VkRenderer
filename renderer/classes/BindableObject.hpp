#ifndef BINDABLE_OBJECT
    #define BINDABLE_OBJECT

#include <vulkan/vulkan.h>

enum Type { cBuffer, cTexture };
class BindableObject 
{ 
    public:
        Type cType;
        VkDescriptorType dType;
        std::string name;
        u_int32_t dSetIndex;
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
