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
	VkDescriptorBufferInfo GetInfo();
};

void Buffer::Create(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkCreateBuffer(init->device, &bufferInfo, nullptr, &buff);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(init->device, buff, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, init->gpu);

	vkAllocateMemory(init->device, &allocInfo, nullptr, &buffMem);
	vkBindBufferMemory(init->device, buff, buffMem, 0);

	info.buffer = buff;
	info.offset = 0;
	info.range = size;
}


void *Buffer::GetData()
{
	return data;
}

void Buffer::SetData(void *data)
{
	std::memcpy(this->data, data, size);
}

void Buffer::Map()
{
	if (!mapped)
	{
		vkMapMemory(init->device, buffMem, 0, size, 0, &data);
		mapped = true;
	}

}

void Buffer::Unmap()
{
	if (mapped)
	{
		vkUnmapMemory(init->device, buffMem);
		mapped = false;
	}
}

#endif
