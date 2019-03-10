#ifndef BUFFER
	#define BUFFER

#include <vulkan/vulkan.h>
#include <cstring>

#include <Initter.hpp>
#include <BindableObject.hpp>

class Buffer : public BindableObject
{
	private:
		void Create(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		Initter *init;
		void *data;
		bool mapped = false;

	public:

	 	VkBuffer buff;
		VkDeviceMemory buffMem;
		VkDeviceSize size;
		VkDescriptorBufferInfo info;

	Buffer (Initter *init, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		this->init = init;
		this->size = size;
		
		Create(usage, properties);
	}

	void SetData(void *data);
	void *GetData();
	void Map();
	void Unmap();
	// void MapPtr(void *ptr);
};

#include <Buffer.cpp>

#endif
