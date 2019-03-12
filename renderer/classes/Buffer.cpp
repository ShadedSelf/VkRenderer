//-----
// Pub
//-----

Buffer Buffer::GetCopy()
{
	Buffer tmpBuff = Buffer(init, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkCommandBuffer tmpCmdBuff = BeginTemporaryCmdBuffer(init->device, init->cmdPool);
		VkBufferCopy region = {};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = size;
		vkCmdCopyBuffer(tmpCmdBuff, buff, tmpBuff.buff, 1, &region);
	EndTemporaryCmdBuffer(init->device, init->cmdPool, init->queue, tmpCmdBuff);
	return tmpBuff;
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
	vkMapMemory(init->device, buffMem, 0, size, 0, &data);
	mapped = true;
}

void Buffer::Unmap()
{
	vkUnmapMemory(init->device, buffMem);
	mapped = false;
}

//-----
// Priv
//-----

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
