#ifndef BINDABLE_BUFFERS
    #define BINDABLE_BUFFERS

#include <vulkan/vulkan.h>

#include <Initter.hpp>
#include <BindableObject.hpp>
#include <Buffer.hpp>
#include <RenderTexture.hpp>
#include <Descriptors.hpp>



struct Container
{
	Type cType;
	VkDescriptorType dType;
	void *vector;
};

class BindableBuffers
{
	private:
		Initter *init;
		std::vector<std::shared_ptr<BindableObject>> buffers;
		// std::vector<Container> container;

		// std::vector<Buffer> uniformBuffers;
		// std::vector<RenderTexture> renderTextures;
		// std::vector<RenderTexture> textures;
		std::vector<std::pair<VkDescriptorType, uint32_t>> diffBuffers;
		std::vector<std::vector<uint32_t>> indices;

	public:
		// BindableBuffers(){}
		BindableBuffers(Initter *init)
		{
			this->init = init;
			// indices.resize(10);
		}

		// void AddUniformBuffer(Buffer buff)
		// {
		// 	uniformBuffers.push_back(buff);
		// 	if (uniformBuffers.size() == 1)
		// 	{
		// 		Container c;
		// 		c.cType = cBuffer;
		// 		c.vector = &uniformBuffers;
		// 		c.dType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		// 		container.push_back(c);
		// 	}
		// }
		void AddUniformBuffer(std::string name, uint32_t size, uint32_t dSetIndex, u_int32_t binding)
		{
			Buffer *buff = new Buffer(init, name, dSetIndex, binding, size, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			buff->cType = cBuffer;
			buff->Map();
			buffers.push_back(std::shared_ptr<BindableObject>(buff));

			bool br = false;
			for (uint32_t i = 0; i < diffBuffers.size(); i++)
			{
				if (diffBuffers[i].first == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				{
					diffBuffers[i].second++;
					br = true;
					return;
				}
			}
			if (!br)
				diffBuffers.push_back(std::pair(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));

			if (dSetIndex >= indices.size())
				indices.resize(dSetIndex + 1);
			if (!indices[dSetIndex].size())
			{
				std::vector<uint32_t> tmp;
				tmp.push_back(buffers.size() - 1);
				indices.insert(indices.begin() + dSetIndex, tmp);
			}
			else
				indices[dSetIndex].push_back(buffers.size() - 1);



			// uniformBuffers.push_back(buff);
			// if (uniformBuffers.size() == 1)
			// {
			// 	Container c;
			// 	c.cType = cBuffer;
			// 	c.vector = &uniformBuffers;
			// 	c.dType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			// 	container.push_back(c);
			// }
		}
		Buffer *GetUniformBuffer(std::string name)
		{
			for (uint32_t i = 0; i < buffers.size(); i++) 
				if (buffers[i]->name == name && buffers[i]->cType == cBuffer)
					return static_cast<Buffer *>(buffers[i].get());
			return nullptr;
		}

		// void AddTexture(RenderTexture tex)
		// {
		// 	textures.push_back(tex);
		// 	if (textures.size() == 1)
		// 	{
		// 		Container c;
		// 		c.cType = cTexture;
		// 		c.vector = &textures;
		// 		c.dType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		// 		container.push_back(c);
		// 	}
		// }
		// void AddRenderTexture(RenderTexture tex)
		// {
		// 	renderTextures.push_back(tex);
		// 	if (renderTextures.size() == 1)
		// 	{
		// 		Container c;
		// 		c.cType = cTexture;
		// 		c.vector = &renderTextures;
		// 		c.dType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		// 		container.push_back(c);
		// 	}
		// }
		// void AddRenderTexture(uint32_t width, uint32_t heigth)
		// {
		// 	RenderTexture tex = RenderTexture(init, width, heigth, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT);
			
		// 	renderTextures.push_back(tex);
		// 	if (renderTextures.size() == 1)
		// 	{
		// 		Container c;
		// 		c.cType = cTexture;
		// 		c.vector = &renderTextures;
		// 		c.dType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		// 		container.push_back(c);
		// 	}
		// }

		void CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage);
};

void BindableBuffers::CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage) //shader stage here
{

// 	// Pool
// 	std::vector<std::pair<VkDescriptorType, uint32_t>> diffBuffers;
// 	for (uint32_t i = 0; i < buffers.size(); i++)
// 	{
// 		bool br = false;
// 		for (uint32_t j = 0; j < diffBuffers.size(); j++)
// 		{
// 			if (diffBuffers[j].first == buffers[i]->dType)
// 			{
// 				diffBuffers[j].second++;
// 				br = true;
// 				break;
// 			}
// 		}
// 		if (br)
// 			continue;
// 		diffBuffers.push_back(std::pair(buffers[i]->dType, 1));
// 	}

// 	std::vector<std::pair<uint32_t, uint32_t>> diffSets;
// 	std::vector<std::vector<uint32_t>> indices;
// 	indices.reserve(10);
// 	for (uint32_t i = 0; i < buffers.size(); i++)
// 	{
// 		bool br = false;
// 		for (uint32_t j = 0; j < diffSets.size(); j++)
// 		{
// 			if (diffSets[j].first == buffers[i]->dSetIndex)
// 			{
// 				diffSets[j].second++;
// 				indices[buffers[i]->dSetIndex].push_back(i);
// 				br = true;
// 				break;
// 			}
// 		}
// 		if (br)
// 			continue;
// 		diffSets.push_back(std::pair(buffers[i]->dSetIndex, 1));
// 		std::vector<uint32_t> tmp;
// 		tmp.push_back(i);
// 		indices.insert(indices.begin() + buffers[i]->dSetIndex, tmp);
// 	}

	VkDescriptorPoolSize *poolSize = new VkDescriptorPoolSize[diffBuffers.size()];
	for (uint32_t i = 0; i < diffBuffers.size(); i++)
	{
		poolSize[i].type = diffBuffers[i].first;
		poolSize[i].descriptorCount = diffBuffers[i].second;
	}

	VkDescriptorPoolCreateInfo dPoolInfo = {};
	dPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	dPoolInfo.maxSets = indices.size();
	dPoolInfo.poolSizeCount = diffBuffers.size();
	dPoolInfo.pPoolSizes =	poolSize;
	VkDescriptorPool dPool;
	vkCreateDescriptorPool(init->device, &dPoolInfo, nullptr, &dPool);
	delete[] poolSize;

	// Layouts
	for (uint32_t i = 0; i < indices.size(); i++)
	{
		VkDescriptorSetLayoutBinding *layoutBindings = new VkDescriptorSetLayoutBinding[indices[i].size()];
		for	(uint32_t j = 0; j < indices[i].size(); j++)
		{
			layoutBindings[i] = {};
		    layoutBindings[i].binding = buffers[indices[i][j]]->binding;
			layoutBindings[i].descriptorType = buffers[indices[i][j]]->dType;
			layoutBindings[i].descriptorCount = 1;
			layoutBindings[i].stageFlags = stage;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = indices[i].size();
		layoutInfo.pBindings = layoutBindings;

		VkDescriptorSetLayout dSetLayout;
		vkCreateDescriptorSetLayout(init->device, &layoutInfo, nullptr, &dSetLayout);
		delete[] layoutBindings;
		dSetLayouts->push_back(dSetLayout);
	}
	
	// Sets
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = dPool;
	allocInfo.descriptorSetCount = indices.size(); 
	allocInfo.pSetLayouts = dSetLayouts->data();
	VkDescriptorSet *dSetsT = new VkDescriptorSet[indices.size()];
	vkAllocateDescriptorSets(init->device, &allocInfo, dSetsT);
	*dSets = std::vector<VkDescriptorSet>(dSetsT, dSetsT + indices.size()); // hmm
	delete[] dSetsT;

	VkWriteDescriptorSet *dWrite = new VkWriteDescriptorSet[buffers.size()];
	for (uint32_t i = 0; i < buffers.size(); i++)
	{
		dWrite[i] = {};
		dWrite[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		dWrite[i].dstSet = (*dSets)[buffers[i]->dSetIndex];
		dWrite[i].dstBinding = buffers[i]->binding;
		dWrite[i].descriptorCount = 1;
		dWrite[i].descriptorType = buffers[i]->dType;
		if (buffers[i]->cType == cBuffer)
		{
			BindableObject *tmp = buffers[i].get();
			Buffer *tmp0 = static_cast<Buffer *>(tmp);
			VkDescriptorBufferInfo asd = tmp0->info;
			dWrite[i].pBufferInfo = &asd;
		}
		else
		{
			// std::vector<RenderTexture> b = (*static_cast<std::vector<RenderTexture> *>(container[i].vector));
			// VkDescriptorImageInfo asd = b[i].info;
			// dWrite[i].pImageInfo = &asd;
		}
	}
	vkUpdateDescriptorSets(init->device, buffers.size(), dWrite, 0, nullptr);

	// while(1);

	// VkDescriptorSetLayoutBinding *layoutBindings = new VkDescriptorSetLayoutBinding[diffSets.size()];
	// for (uint32_t i = 0; i < buffers.size(); ++i)
	// {
	// 	layoutBindings[i] = {};
	//     layoutBindings[i].binding = buffers[i]->dSetIndex;
	// 	layoutBindings[i].descriptorType = buffers[i]->dType;
	// 	layoutBindings[i].descriptorCount = 1;
	// 	layoutBindings[i].stageFlags = stage;
	// }

	// VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	// layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	// layoutInfo.bindingCount = buffers.size();
	// layoutInfo.pBindings = layoutBindings;

	// VkDescriptorSetLayout dSetLayout;
	// vkCreateDescriptorSetLayout(init->device, &layoutInfo, nullptr, &dSetLayout);
	// delete[] layoutBindings;
	// dSetLayouts->push_back(dSetLayout);

	// // Sets
	// VkDescriptorSetAllocateInfo allocInfo = {};
	// allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	// allocInfo.descriptorPool = dPool;
	// allocInfo.descriptorSetCount = sizes.size(); 
	// allocInfo.pSetLayouts = dSetLayouts->data();
	// VkDescriptorSet *dSetsT = new VkDescriptorSet[sizes.size()];
	// vkAllocateDescriptorSets(init->device, &allocInfo, dSetsT);
	// *dSets = std::vector<VkDescriptorSet>(dSetsT, dSetsT + sizes.size()); // hmm
	// // delete[] dSetsT;

	// VkWriteDescriptorSet *dWrite = new VkWriteDescriptorSet[buffers.size()];
	// for (uint32_t i = 0; i < buffers.size(); i++)
	// {
	// 	dWrite[i] = {};
	// 	dWrite[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// 	dWrite[i].dstSet = (*dSets)[i];
	// 	dWrite[i].dstBinding = bufers[i]->binding;
	// 	dWrite[i].descriptorCount = 1;
	// 	dWrite[i].descriptorType = buffers[i]->dType;
	// 	if (container[i].cType == cBuffer)
	// 	{
	// 		std::vector<Buffer> b = (*static_cast<std::vector<Buffer> *>(container[i].vector));
	// 		VkDescriptorBufferInfo asd = b[i].info;
	// 		dWrite[i].pBufferInfo = &asd;
	// 	}
	// 	else
	// 	{
	// 		std::vector<RenderTexture> b = (*static_cast<std::vector<RenderTexture> *>(container[i].vector));
	// 		VkDescriptorImageInfo asd = b[i].info;
	// 		dWrite[i].pImageInfo = &asd;
	// 	}
	// }
	// vkUpdateDescriptorSets(init->device, len, dWrite, 0, nullptr);
	// delete[] dWrite;












	// VkDescriptorPoolSize *poolSize = new VkDescriptorPoolSize[container.size()];
	// for (uint32_t i = 0; i < container.size(); i++)
	// {
	// 	poolSize[i].type = container[i].dType;
	// 	uint32_t len = (container[i].cType == cBuffer)
	// 					? static_cast<std::vector<Buffer> *>(container[i].vector)->size()
	// 					: static_cast<std::vector<RenderTexture> *>(container[i].vector)->size();
	// 	poolSize[i].descriptorCount = len;
	// }

	// // Pool
	// VkDescriptorPoolSize *poolSize = new VkDescriptorPoolSize[container.size()];
	// for (uint32_t i = 0; i < container.size(); i++)
	// {
	// 	poolSize[i].type = container[i].dType;
	// 	uint32_t len = (container[i].cType == cBuffer)
	// 					? static_cast<std::vector<Buffer> *>(container[i].vector)->size()
	// 					: static_cast<std::vector<RenderTexture> *>(container[i].vector)->size();
	// 	poolSize[i].descriptorCount = len;
	// }

	// VkDescriptorPoolCreateInfo dPoolInfo = {};
	// dPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	// dPoolInfo.maxSets = container.size();
	// dPoolInfo.poolSizeCount = container.size();
	// dPoolInfo.pPoolSizes =	poolSize;
	// VkDescriptorPool dPool;
	// vkCreateDescriptorPool(init->device, &dPoolInfo, nullptr, &dPool);
	// delete[] poolSize;

	// // Lyouts
	// for (uint32_t i = 0; i < container.size(); i++)
	// {
	// 	uint32_t len = (container[i].cType == cBuffer)
	// 			? static_cast<std::vector<Buffer> *>(container[i].vector)->size()
	// 			: static_cast<std::vector<RenderTexture> *>(container[i].vector)->size();
	// 	uint32_t *bindings = new uint32_t[len];
	// 	for (uint32_t j = 0; j < len; ++j)
	// 		bindings[j] = j;//uniformBuffers[j].binding;
			
	// 	VkDescriptorSetLayout layout = CreateDescriptorSetLayout(init->device, container[i].dType, stage, bindings, len);
	// 	dSetLayouts->push_back(layout);
	// 	delete[] bindings;
	// }

	// // Sets
	// VkDescriptorSetAllocateInfo allocInfo = {};
	// allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	// allocInfo.descriptorPool = dPool;
	// allocInfo.descriptorSetCount = container.size(); 
	// allocInfo.pSetLayouts = dSetLayouts->data();
	// VkDescriptorSet *dSetsT = new VkDescriptorSet[container.size()];
	// vkAllocateDescriptorSets(init->device, &allocInfo, dSetsT);
	// *dSets = std::vector<VkDescriptorSet>(dSetsT, dSetsT + container.size()); // hmm
	// // delete[] dSetsT;

	// for (uint32_t i = 0; i < container.size(); i++)
	// {
	// 	uint32_t len = (container[i].cType == cBuffer)
	// 					? static_cast<std::vector<Buffer> *>(container[i].vector)->size()
	// 					: static_cast<std::vector<RenderTexture> *>(container[i].vector)->size();
	// 	VkWriteDescriptorSet *dWrite = new VkWriteDescriptorSet[len];
	// 	for	(uint32_t j = 0; j < len; j++)
	// 	{
	// 		dWrite[j] = {};
	// 		dWrite[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// 		dWrite[j].dstSet = (*dSets)[i];
	// 		dWrite[j].dstBinding = j;
	// 		dWrite[j].descriptorCount = 1;
	// 		dWrite[j].descriptorType = container[i].dType;
	// 		if (container[i].cType == cBuffer)
	// 		{
	// 			std::vector<Buffer> b = (*static_cast<std::vector<Buffer> *>(container[i].vector));
	// 			VkDescriptorBufferInfo asd = b[j].info;
	// 			dWrite[j].pBufferInfo = &asd;
	// 		}
	// 		else
	// 		{
	// 			std::vector<RenderTexture> b = (*static_cast<std::vector<RenderTexture> *>(container[i].vector));
	// 			VkDescriptorImageInfo asd = b[j].info;
	// 			dWrite[j].pImageInfo = &asd;
	// 		}
	// 	}
	// 	vkUpdateDescriptorSets(init->device, len, dWrite, 0, nullptr);
	// 	delete[] dWrite;
	// }
}


#endif
