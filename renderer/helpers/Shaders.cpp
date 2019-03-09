#include <vulkan/vulkan.h>
#include <Shaders.hpp>

#include <stdio.h>
#include <cstdlib>

int LoadShader(char **buff, const char *path)
{
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *source = new char[fsize + 1];
	fread(source, fsize, 1, f);
	fclose(f);

	source[fsize] = 0;
	*buff = source;
	return fsize;
}

VkShaderModule GetShaderModule(VkDevice device, const char *path)
{
    char *code;
	size_t size = LoadShader(&code, path);

    VkShaderModuleCreateInfo moduleCreateInfo;
    VkShaderModule module;

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = nullptr;

    moduleCreateInfo.codeSize = size;
    moduleCreateInfo.pCode = (uint32_t*)code;
    moduleCreateInfo.flags = 0;

    vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &module);
	delete[] code;
    return module;
}
