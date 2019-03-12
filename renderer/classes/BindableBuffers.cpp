
void BindableBuffers::Flush()
{
	buffers.clear();
	diffBuffers.clear();
}

void BindableBuffers::AddBuffer(std::string name, uint32_t size, BufferType type, u_int32_t binding, bool map, bool deviceOnly)
{
	VkMemoryPropertyFlags pFlags;
	if (deviceOnly)
		pFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	else
		pFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		
	VkBufferUsageFlags uFlags;
	if (type == UniformBuffer)
		uFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	else
		uFlags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	Buffer *buff = new Buffer(init, size, uFlags, pFlags);
	buff->buffer = true;
	if (map)
		buff->Map();
	Add(buff, name, type, binding);
}

void BindableBuffers::AddTexture(std::string name, uint32_t width, u_int32_t height, BufferType type, u_int32_t binding)
{
	RenderTexture *buff = new RenderTexture(init, width, height, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT);
	buff->buffer = false;
	Add(buff, name, type, binding);
}

void BindableBuffers::AddTexture(std::string name, BufferType type, u_int32_t binding, RenderTexture tex)
{
	RenderTexture *buff = new RenderTexture(tex);
	buff->buffer = false;
	Add(buff, name, type, binding);
}

void BindableBuffers::Add(BindableObject *buff, std::string name, BufferType type, u_int32_t binding)
{
	buff->dType = type;
	buff->name = name;
	buff->binding = binding;

	buffers.push_back(std::unique_ptr<BindableObject>(buff));

	for (uint32_t i = 0; i < diffBuffers.size(); i++)
		if (diffBuffers[i].type == type)
			{ diffBuffers[i].count++; return; }
	diffBuffers.push_back(BufferTrack(type, 1));
}

Buffer *BindableBuffers::GetBuffer(std::string name)
{
	for (uint32_t i = 0; i < buffers.size(); i++) 
		if (buffers[i]->name == name && buffers[i]->buffer)
			return static_cast<Buffer *>(buffers[i].get());
	return nullptr;
}

void BindableBuffers::CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage)
{
	// Pool
	VkDescriptorPoolSize *poolSize = new VkDescriptorPoolSize[diffBuffers.size()];
	for (uint32_t i = 0; i < diffBuffers.size(); i++)
	{
		poolSize[i].type = (VkDescriptorType)diffBuffers[i].type;
		poolSize[i].descriptorCount = diffBuffers[i].count;
	}

	VkDescriptorPoolCreateInfo dPoolInfo = {};
	dPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	dPoolInfo.maxSets = 1;
	dPoolInfo.poolSizeCount = diffBuffers.size();
	dPoolInfo.pPoolSizes =	poolSize;
	
	VkDescriptorPool dPool;
	vkCreateDescriptorPool(init->device, &dPoolInfo, nullptr, &dPool);
	delete[] poolSize;

	// Layout
	VkDescriptorSetLayoutBinding *layoutBindings = new VkDescriptorSetLayoutBinding[buffers.size()];
	for	(uint32_t i = 0; i < buffers.size(); i++)
	{
		layoutBindings[i] = {};
		layoutBindings[i].binding = buffers[i]->binding;
		layoutBindings[i].descriptorType = (VkDescriptorType)buffers[i]->dType;
		layoutBindings[i].descriptorCount = 1;
		layoutBindings[i].stageFlags = stage;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = buffers.size();
	layoutInfo.pBindings = layoutBindings;

	VkDescriptorSetLayout dSetLayout;
	vkCreateDescriptorSetLayout(init->device, &layoutInfo, nullptr, &dSetLayout);
	dSetLayouts->push_back(dSetLayout);
	delete[] layoutBindings;
	
	// Set
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = dPool;
	allocInfo.descriptorSetCount = 1; 
	allocInfo.pSetLayouts = &dSetLayout;

	VkDescriptorSet dSet;;
	vkAllocateDescriptorSets(init->device, &allocInfo, &dSet);
	dSets->push_back(dSet);

	VkWriteDescriptorSet *dWrite = new VkWriteDescriptorSet[buffers.size()];
	for (uint32_t i = 0; i < buffers.size(); i++)
	{
		dWrite[i] = {};
		dWrite[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		dWrite[i].dstSet = dSet;
		dWrite[i].dstBinding = buffers[i]->binding;
		dWrite[i].descriptorCount = 1;
		dWrite[i].descriptorType = (VkDescriptorType)buffers[i]->dType;

		BindableObject *tmp = buffers[i].get();
		if (buffers[i]->buffer)
		{
			VkDescriptorBufferInfo *asd = new VkDescriptorBufferInfo(static_cast<Buffer *>(tmp)->info);
			dWrite[i].pBufferInfo = asd;
		}
		else
		{
			VkDescriptorImageInfo *asd = new VkDescriptorImageInfo(static_cast<RenderTexture *>(tmp)->info);
			dWrite[i].pImageInfo = asd;
		}
	}
	vkUpdateDescriptorSets(init->device, buffers.size(), dWrite, 0, nullptr);
	for (uint32_t i = 0; i < buffers.size(); i++)
	{
		if (dWrite[i].pBufferInfo)
			delete dWrite[i].pBufferInfo;
		if (dWrite[i].pImageInfo)
			delete dWrite[i].pImageInfo;
	}
	delete[] dWrite;
}
