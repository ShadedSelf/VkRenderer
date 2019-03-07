#ifndef BINDABLE_OBJECT_HOLDER
	#define BINDABLE_OBJECT_HOLDER

#include <vulkan/vulkan.h>

#include <vector>
#include <memory>

#include <BindableObject.hpp>

class BindableObjectHolder
{
	private:
        std::vector<std::unique_ptr<BindableObject>> objects;

	public:
        VkDescriptorType type;
        uint32_t index;
        uint32_t size;

        BindableObjectHolder(VkDescriptorType type, uint32_t index)
        {
            this->type = type;
            this->index = index;
        }

        void Add(BindableObject *ptr)
        {
            objects.push_back(std::unique_ptr<BindableObject>(ptr));
            size = objects.size();
            // size++;
        }

        BindableObject * operator [](uint32_t index) 
        {
           return objects[index].get();
        }

        Buffer GetBuffer(uint32_t index)
        {
            return *static_cast<Buffer *>(objects[index].get());
        }

        RenderTexture GetTexture(uint32_t index)
        {
            return *static_cast<RenderTexture *>(objects[index].get());
        }
};


#endif
