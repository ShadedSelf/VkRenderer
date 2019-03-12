#ifndef BUFFER
	#define BUFFER

#include <vulkan/vulkan.h>
#include <cstring>

#include <Initter.hpp>
#include <BindableObject.hpp>
#include <CommandBuffers.hpp>

class Buffer : public BindableObject
{
	private:
		Initter *init;
		void *data;
		bool mapped = false;

	 	VkBuffer buff;
		VkDeviceMemory buffMem;
		VkDeviceSize size;

		void Create(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	public:
		VkDescriptorBufferInfo info;

	Buffer(Initter *init, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		this->init = init;
		this->size = size;
		Create(usage, properties);
	}
	~Buffer()
	{
		vkFreeMemory(init->device, buffMem, nullptr);
		vkDestroyBuffer(init->device, buff, nullptr);
	}

	void SetData(void *data);
	void *GetData();
	void Map();
	void Unmap();
	Buffer GetCopy();
	// void MapPtr(void *ptr);
};

#include <Buffer.cpp>

#endif
