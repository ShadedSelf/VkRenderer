#ifndef BINDABLE_BUFFERS
    #define BINDABLE_BUFFERS

#include <vulkan/vulkan.h>

#include <Initter.hpp>
#include <BindableObject.hpp>
#include <Buffer.hpp>
#include <RenderTexture.hpp>

struct BufferTrack
{
	BufferType type;
	uint32_t count;
	BufferTrack(BufferType type, uint32_t count)
	{
		this->type = type;
		this->count = count;
	}
};
#include <cstdio>
class BindableBuffers
{
	private:
		Initter *init;
		std::vector<std::unique_ptr<BindableObject>> buffers;
		std::vector<BufferTrack> diffBuffers;

		void Add(BindableObject *buff, std::string name, BufferType type, u_int32_t binding);

	public:
		BindableBuffers(Initter *init)
		{
			this->init = init;
		}
		~BindableBuffers()
		{
			Flush();
		}
		void Flush();
		void AddBuffer(std::string name, uint32_t size, BufferType type, u_int32_t binding, bool map, bool deviceOnly);
		void AddTexture(std::string name, uint32_t width, u_int32_t height, BufferType type, u_int32_t binding);
		void AddTexture(std::string name, BufferType type, u_int32_t binding, RenderTexture tex);
		Buffer *GetBuffer(std::string name);
		void CreateDescriptors(std::vector<VkDescriptorSet> *dSets, std::vector<VkDescriptorSetLayout> *dSetLayouts, VkShaderStageFlagBits stage);
};

#include <BindableBuffers.cpp>

#endif
