#ifndef BINDABLE_BUFFERS
    #define BINDABLE_BUFFERS

#include <vulkan/vulkan.h>

#include <Initter.hpp>
#include <BindableObject.hpp>
#include <Buffer.hpp>
#include <RenderTexture.hpp>
#include <Descriptors.hpp>


class BindableBuffers
{
	private:
		Initter *init;
		std::vector<std::shared_ptr<BindableObject>> buffers;

		std::vector<std::pair<VkDescriptorType, uint32_t>> diffBuffers;
		std::vector<std::vector<uint32_t>> indices;

	public:
		// BindableBuffers(){}
		BindableBuffers(Initter *init)
		{
			this->init = init;
		}

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

	}
		Buffer *GetUniformBuffer(std::string name)
		{
			for (uint32_t i = 0; i < buffers.size(); i++) 
				if (buffers[i]->name == name && buffers[i]->cType == cBuffer)
					return static_cast<Buffer *>(buffers[i].get());
			return nullptr;
		}

		void CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage);
};

void BindableBuffers::CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage) //shader stage here
{

	// Pool
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
}


#endif
