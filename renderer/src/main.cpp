#include <cstdlib>
#include <cstring>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb_image_write.h>

#include <iostream>
#include <vector>

#include <Common.hpp>

#include <Descriptors.hpp>
#include <Shaders.hpp>

#include <ComputePipeline.hpp>
#include <Buffer.hpp>
#include <RenderTexture.hpp>
#include <BindableObject.hpp>
#include <BindableObjectHolder.hpp>

typedef struct
{
    VkImage image;
    VkCommandBuffer cmd;
    VkImageView view;
} SwapchainBuffers;

struct test
{
    GLFWwindow* window;
    VkSurfaceKHR surface;

    VkInstance inst;
    VkPhysicalDevice gpu;
    VkDevice device;
    VkQueue queue;

    VkQueueFamilyProperties *queueProps;
    uint32_t graphicsQueueIndex;

    uint32_t enabledExtensionCount;
    const char *extensionNames[64];

    int width, height;
    VkFormat format;
    VkColorSpaceKHR colorSpace;

	uint32_t queueCount;
    uint32_t swapchainImageCount;
    VkSwapchainKHR swapchain;
    SwapchainBuffers *swapChainbuffers;

    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline pipeline;

	VkExtent2D swapchainExtent;
    VkFramebuffer *framebuffers;
	VkCommandBuffer *commandBuffers;

	VkDescriptorPool descriptorPool;
	VkDescriptorSet *descriptorSets;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence fence;

	VkDescriptorSetLayout *graphicsDescriptorSetLayouts;






	VkCommandPool cmdPool;
	VkFence	computeFence;

	VkDescriptorPool computeDescriptorPool;
	VkDescriptorSetLayout *computeDescriptorSetLayouts;
	VkDescriptorSet *computeDescriptorSets;
	VkPipelineLayout computePipelineLayout;

	std::vector<std::unique_ptr<BindableObjectHolder>> bindableObjects;
	BindableObjectHolder uniformBuffers_h;
	BindableObjectHolder storageBuffers_h;
	BindableObjectHolder renderTextures_h;
	BindableObjectHolder textures_h;

	std::vector<Buffer> uniformBuffers;
	std::vector<Buffer> storageBuffers;
	std::vector<RenderTexture> renderTextures;
	std::vector<RenderTexture> textures;

	std::vector<ComputePipeline> computePipelines;
};

struct test *test;
Globals globals;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void CreateWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    test->window = glfwCreateWindow(test->width, test->height, "Test", nullptr, nullptr);
    glfwSetWindowUserPointer(test->window, test);
    glfwSetKeyCallback(test->window, key_callback);
}

uint32_t validationLayerCount;
char *validationLayers;

void InitInstance()
{
	uint32_t requiredExtensionCount = 0;
	const char **requiredExtensions = nullptr;
    requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
    // for (int i = 0; i < requiredExtensionCount; i++)
    //     test->extensionNames[test->enabledExtensionCount++] = requiredExtensions[i];

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pNext = nullptr;
	app.pApplicationName = "Test";
    app.applicationVersion = 0;
    app.pEngineName = "Test";
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.pApplicationInfo = &app;
    instanceInfo.enabledExtensionCount = requiredExtensionCount;
    instanceInfo.ppEnabledExtensionNames = requiredExtensions;
    vkCreateInstance(&instanceInfo, nullptr, &test->inst);

	uint32_t gpuCount;
    vkEnumeratePhysicalDevices(test->inst, &gpuCount, nullptr);
	VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[gpuCount];
    vkEnumeratePhysicalDevices(test->inst, &gpuCount, physicalDevices);
	test->gpu = physicalDevices[0];
    delete[] physicalDevices;

    uint32_t deviceExtensionCount = 0;
    test->enabledExtensionCount = 0;

    vkEnumerateDeviceExtensionProperties(test->gpu, nullptr, &deviceExtensionCount, nullptr);
	VkExtensionProperties *deviceExtensions = new VkExtensionProperties[deviceExtensionCount];
    vkEnumerateDeviceExtensionProperties(test->gpu, nullptr, &deviceExtensionCount, deviceExtensions);

    for (int i = 0; i < deviceExtensionCount; i++)
        if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, deviceExtensions[i].extensionName))
            test->extensionNames[test->enabledExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    delete[] deviceExtensions;

    vkGetPhysicalDeviceQueueFamilyProperties(test->gpu, &test->queueCount, nullptr);
	test->queueProps = new VkQueueFamilyProperties[test->queueCount];
    vkGetPhysicalDeviceQueueFamilyProperties(test->gpu, &test->queueCount, test->queueProps);
}

void InitDevice()
{
	test->graphicsQueueIndex = 0;

    float queuePriorities[1] = {0.0};
    VkDeviceQueueCreateInfo queue = {};
    queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue.pNext = nullptr;
    queue.queueFamilyIndex = test->graphicsQueueIndex;
    queue.queueCount = 1;
    queue.pQueuePriorities = queuePriorities;


    VkPhysicalDeviceFeatures features = {};

    VkDeviceCreateInfo device = {};
    device.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device.pNext = nullptr;
    device.queueCreateInfoCount = 1;
    device.pQueueCreateInfos = &queue;
    device.enabledLayerCount = 0;
    device.ppEnabledLayerNames = nullptr;
    device.enabledExtensionCount = test->enabledExtensionCount;
    device.ppEnabledExtensionNames = (const char *const *)test->extensionNames;
    device.pEnabledFeatures = &features;

    vkCreateDevice(test->gpu, &device, nullptr, &test->device);
}

void InitVk()
{
    glfwCreateWindowSurface(test->inst, test->window, nullptr, &test->surface);

    vkGetDeviceQueue(test->device, test->graphicsQueueIndex, 0, &test->queue);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(test->gpu, test->surface, &formatCount, nullptr);
	VkSurfaceFormatKHR *surfFormats = new VkSurfaceFormatKHR[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(test->gpu, test->surface, &formatCount, surfFormats);

    test->format = surfFormats[2].format;
    test->colorSpace = surfFormats[2].colorSpace;
	delete[] surfFormats;
}

void CreateSwapchain()
{
	VkBool32 surfaceSupport;
	vkGetPhysicalDeviceSurfaceSupportKHR(test->gpu, 0, test->surface, &surfaceSupport);

    VkSurfaceCapabilitiesKHR surfCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( test->gpu, test->surface, &surfCapabilities);

    test->swapchainExtent.width = test->width;
    test->swapchainExtent.height = test->height;

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t desiredNumOfSwapchainImages = surfCapabilities.minImageCount;

	VkSurfaceTransformFlagBitsKHR preTransform;
    preTransform = surfCapabilities.currentTransform;

    VkSwapchainCreateInfoKHR swapchain = {};
    swapchain.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain.pNext = nullptr;
    swapchain.surface = test->surface;
    swapchain.minImageCount = desiredNumOfSwapchainImages;
    swapchain.imageFormat = test->format;
    swapchain.imageColorSpace = test->colorSpace;
    swapchain.imageExtent = test->swapchainExtent;
    swapchain.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain.preTransform = preTransform;
    swapchain.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain.imageArrayLayers = 1;
    swapchain.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain.queueFamilyIndexCount = 0;
    swapchain.pQueueFamilyIndices = nullptr;
    swapchain.presentMode = swapchainPresentMode;
    swapchain.oldSwapchain = VK_NULL_HANDLE;
    swapchain.clipped = true;

    vkCreateSwapchainKHR(test->device, &swapchain, nullptr, &test->swapchain);

	vkGetSwapchainImagesKHR(test->device, test->swapchain, &test->swapchainImageCount, nullptr);
    VkImage *swapchainImages = new VkImage[test->swapchainImageCount];
    vkGetSwapchainImagesKHR(test->device, test->swapchain, &test->swapchainImageCount, swapchainImages);
    test->swapChainbuffers = new SwapchainBuffers[test->swapchainImageCount];

    for (int i = 0; i < test->swapchainImageCount; i++)
	{
        VkImageViewCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageInfo.pNext = nullptr;
        imageInfo.format = test->format;

        imageInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageInfo.components.a = VK_COMPONENT_SWIZZLE_A;

        imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
       	imageInfo.subresourceRange.baseMipLevel = 0;
        imageInfo.subresourceRange.levelCount = 1;
        imageInfo.subresourceRange.baseArrayLayer = 0;
        imageInfo.subresourceRange.layerCount = 1;

        imageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D,
        imageInfo.flags = 0,
		imageInfo.image = swapchainImages[i];

        test->swapChainbuffers[i].image = swapchainImages[i];


        vkCreateImageView(test->device, &imageInfo, nullptr, &test->swapChainbuffers[i].view);
    }
	delete[] swapchainImages;
}

void CreateRenderPass()
{
	VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = test->format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	vkCreateRenderPass(test->device, &renderPassInfo, nullptr, &test->renderPass);
}

void CreateGraphicsPipe(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	VkShaderModule vertexShader = GetShaderModule(test->device, vertexShaderPath);
	VkShaderModule fragmentShader = GetShaderModule(test->device, fragmentShaderPath);

	VkPipelineShaderStageCreateInfo shaderStages[2] = {};
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShader;
    shaderStages[0].pName = "main";

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentShader;
    shaderStages[1].pName = "main";

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)test->width;
	viewport.height = (float)test->height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = test->swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 0; //2
	dynamicState.pDynamicStates = dynamicStates;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 2;
	pipelineLayoutInfo.pSetLayouts = test->graphicsDescriptorSetLayouts;
	vkCreatePipelineLayout(test->device, &pipelineLayoutInfo, nullptr, &test->pipelineLayout);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = test->pipelineLayout;
	pipelineInfo.renderPass = test->renderPass;
	pipelineInfo.subpass = 0;
	vkCreateGraphicsPipelines(test->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &test->pipeline);

	vkDestroyShaderModule(test->device, fragmentShader, nullptr);
    vkDestroyShaderModule(test->device, vertexShader, nullptr);
}

void CreateFrameBuffers()
{
	test->framebuffers = new VkFramebuffer[test->swapchainImageCount];
	VkImageView *tmp = new VkImageView[test->swapchainImageCount];
	for (int i = 0; i < test->swapchainImageCount; ++i)
		tmp[i] = test->swapChainbuffers[i].view;
	for (int i = 0; i < test->swapchainImageCount; ++i)
	{
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = test->renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &tmp[i];
		framebufferInfo.width = test->width;
		framebufferInfo.height = test->height;
		framebufferInfo.layers = 1;

		vkCreateFramebuffer(test->device, &framebufferInfo, nullptr, &test->framebuffers[i]);
	}
	delete[] tmp;
}

void CreateCommandPools()
{
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = test->graphicsQueueIndex;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	vkCreateCommandPool(test->device, &cmdPoolInfo, nullptr, &test->cmdPool);
}

void CreateGraphicsCommandBuffers()
{
	test->commandBuffers = new VkCommandBuffer[test->swapchainImageCount];
	for (int i = 0; i < test->swapchainImageCount; ++i)
	{
		test->commandBuffers[i] = AllocateCommandBuffer(test->device, test->cmdPool);

		RecorderParams params;
		params.renderPass = test->renderPass;
		params.frameBuffer = test->framebuffers[i];
		params.extent = test->swapchainExtent;
		RecordCommandBuffer(test->commandBuffers[i], test->pipeline, test->pipelineLayout, test->descriptorSets, 2, VK_PIPELINE_BIND_POINT_GRAPHICS, params);
	}
}

void CreateSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(test->device, &semaphoreInfo, nullptr, &test->imageAvailableSemaphore);
    vkCreateSemaphore(test->device, &semaphoreInfo, nullptr, &test->renderFinishedSemaphore);

	VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vkCreateFence(test->device, &fenceInfo, nullptr, &test->fence);
	vkResetFences(test->device, 1, &test->fence);
}

void CreateGraphicsDescriptors()
{
	// Pool
	VkDescriptorPoolSize poolSize[2] = {};
	poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize[0].descriptorCount = 1;
	poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSize;
	poolInfo.maxSets = 2;
	vkCreateDescriptorPool(test->device, &poolInfo, nullptr, &test->descriptorPool);

	// Layouts
	test->graphicsDescriptorSetLayouts = new VkDescriptorSetLayout[2];

	uint32_t bufferBindings[1] = { 0 };
	test->graphicsDescriptorSetLayouts[0] = CreateDescriptorSetLayout(test->device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, &bufferBindings[0], 1);
	uint32_t textureBindingd[2] = { 0 };
	test->graphicsDescriptorSetLayouts[1]  = CreateDescriptorSetLayout(test->device, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, &textureBindingd[0], 1);

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = test->uniformBuffers[0].buff;
	bufferInfo.offset = 0;
	bufferInfo.range = test->uniformBuffers[0].size;

	SetCreateParams *params = new SetCreateParams[3];
	params[0].dType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	params[0].dstBinding = 0;
	params[0].bufferInfo = bufferInfo;
	params[0].dSetIndex = 0;

	params[1].dType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	params[1].dstBinding = 0;
	params[1].imageInfo = test->renderTextures[0].descriptor;
	params[1].dSetIndex = 1;

	test->descriptorSets = CreateDesciptorSets(test->device, test->graphicsDescriptorSetLayouts, 2, test->descriptorPool, 2, params);
	delete[] params;
}

void desc(VkDescriptorSetLayout *layouts, VkDescriptorPool *dPool, VkShaderStageFlagBits stage)
{
	// Pool
	VkDescriptorPoolSize *poolSize = new VkDescriptorPoolSize[test->bindableObjects.size()];
	for (uint32_t i = 0; i < test->bindableObjects.size(); i++)
	{
		poolSize[i].type = test->bindableObjects[i]->type;
		poolSize[i].descriptorCount = test->bindableObjects[i]->size;
	}

	VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.maxSets = test->bindableObjects.size();
	descriptorPoolInfo.poolSizeCount = test->bindableObjects.size();
	descriptorPoolInfo.pPoolSizes =	poolSize;
	vkCreateDescriptorPool(test->device, &descriptorPoolInfo, nullptr, dPool);

	// Layouts
	for (uint32_t i = 0; i < test->bindableObjects.size(); i++)
	{
		uint32_t *bindings = new uint32_t[test->bindableObjects[i]->size];
		for (uint32_t j = 0; j < test->bindableObjects[i]->size; ++j)
			bindings[j] = j;
		layouts[test->bindableObjects[i]->index] = CreateDescriptorSetLayout(test->device, test->bindableObjects[i]->type, stage, bindings, test->bindableObjects[i]->size);
		delete[] bindings;
	}
	
	// Sets
	SetCreateParams *params = new SetCreateParams[test->renderTextures.size() + test->uniformBuffers.size() + test->storageBuffers.size() + test->textures.size()];

	uint32_t gIndex = 0;
	// for (uint32_t i = 0; i < test->bindableObjects.size(); i++)
	// {
	// 	BindableObjectHolder *holder = test->bindableObjects[i].get();
	// 	for (uint32_t j = 0; j < holder->size; ++j)
	// 	{
	// 		params[gIndex].dType = holder->type;
	// 		params[gIndex].dstBinding = j;
	// 		params[gIndex].dSetIndex = holder->index;

	// 		BindableObject *tmp = (*holder)[j];
	// 		if (holder->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || holder->type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
	// 			params[gIndex].bufferInfo = static_cast<Buffer *>(tmp)->GetInfo();
	// 		else
	// 			params[gIndex].imageInfo = static_cast<RenderTexture *>(tmp)->GetInfo();

	// 		gIndex++;
	// 	}
	// }

	for (uint32_t i = 0; i < test->uniformBuffers.size(); ++i)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = test->uniformBuffers[i].buff;
		bufferInfo.offset = 0;
		bufferInfo.range = test->uniformBuffers[i].size;

		params[gIndex].dType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		params[gIndex].dstBinding = i;
		params[gIndex].bufferInfo = bufferInfo;
		params[gIndex].dSetIndex = 1;
		gIndex++;
	}

	for (uint32_t i = 0; i < test->storageBuffers.size(); ++i)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = test->storageBuffers[i].buff;
		bufferInfo.offset = 0;
		bufferInfo.range = test->storageBuffers[i].size;

		params[gIndex].dType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		params[gIndex].dstBinding = i;
		params[gIndex].bufferInfo = bufferInfo;
		params[gIndex].dSetIndex = 2;
		gIndex++;
	}

	for (uint32_t i = 0; i < test->renderTextures.size(); ++i)
	{
		params[gIndex].dType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		params[gIndex].dstBinding = i;
		params[gIndex].imageInfo = test->renderTextures[i].descriptor;
		params[gIndex].dSetIndex = 0;
		gIndex++;
	}

	for (uint32_t i = 0; i < test->textures.size(); ++i)
	{
		// test->textures[i].descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		params[gIndex].dType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		params[gIndex].dstBinding = i;
		params[gIndex].imageInfo = test->textures[i].descriptor;
		params[gIndex].dSetIndex = 3;
		gIndex++;
	}

	test->computeDescriptorSets = CreateDesciptorSets(test->device, layouts, test->bindableObjects.size(), *dPool, gIndex, params);
	delete[] params;
}

void BindComputeData()
{
	test->computeDescriptorSetLayouts = new VkDescriptorSetLayout[test->bindableObjects.size()];
	desc(test->computeDescriptorSetLayouts, &test->computeDescriptorPool, VK_SHADER_STAGE_COMPUTE_BIT);
}

void DestroyComputePipes()
{
	while (test->computePipelines.size() > 0)
		test->computePipelines.pop_back();
}

GLFWwindow *Initialize(uint32_t x, uint32_t y)
{
	test = (struct test*)calloc(1, sizeof(struct test));

	// Globals ss = Init::Init();

	test->width = x;
    test->height = y;

	glfwInit();
	CreateWindow();

    InitInstance();
	InitDevice();
	InitVk();

	CreateSwapchain();
	CreateRenderPass();
	CreateCommandPools();

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkCreateFence(test->device, &fenceCreateInfo, nullptr, &test->computeFence);

	globals.device  = test->device;
	globals.cmdPool = test->cmdPool;
	globals.width   = test->width;
	globals.height  = test->height;
	globals.gpu     = test->gpu;
	globals.queue   = test->queue;

	test->uniformBuffers_h = BindableObjectHolder(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);
	test->storageBuffers_h = BindableObjectHolder(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2);
	test->renderTextures_h = BindableObjectHolder(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,  0);
	test->textures_h 	   = BindableObjectHolder(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3);

	test->bindableObjects.push_back(std::unique_ptr<BindableObjectHolder>(&test->uniformBuffers_h));
	test->bindableObjects.push_back(std::unique_ptr<BindableObjectHolder>(&test->storageBuffers_h));
	test->bindableObjects.push_back(std::unique_ptr<BindableObjectHolder>(&test->renderTextures_h));
	test->bindableObjects.push_back(std::unique_ptr<BindableObjectHolder>(&test->textures_h));

	return test->window;
}

void AddUniformBuffer(uint32_t size)
{
	VkDeviceSize bufferSize = size;
	Buffer tmp = Buffer(globals, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	test->uniformBuffers.push_back(tmp);

	Buffer *ptr = new Buffer(globals, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	test->uniformBuffers_h.Add(ptr);
}

void AddStorageBuffer(uint32_t size)
{
	VkDeviceSize bufferSize = size;
	Buffer tmp = Buffer(globals, bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	test->storageBuffers.push_back(tmp);

	test->storageBuffers_h.Add(new Buffer(globals, bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
}

void AddRenderTexture(uint32_t width, uint32_t height)
{
	RenderTexture texture = RenderTexture(width, height, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT, globals);
	test->renderTextures.push_back(texture);

	test->renderTextures_h.Add(new RenderTexture(width, height, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT, globals));
}

void AddTextureFromData(uint32_t width, uint32_t height, void *data)
{
	RenderTexture texture = RenderTexture(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, globals);

	VkDeviceSize size = width * height * sizeof(uint8_t) * 4;
	Buffer tmpBuff = Buffer(globals, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	tmpBuff.Map();
		tmpBuff.SetData(data);
	tmpBuff.Unmap();

	VkCommandBuffer tmp = BeginTemporaryCmdBuffer(test->device, test->cmdPool);
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset.x = 0;
		region.imageOffset.y = 0;
		region.imageOffset.z = 0;
		region.imageExtent.width = width;
		region.imageExtent.height = height;
		region.imageExtent.depth = 1;

		vkCmdCopyBufferToImage(tmp, tmpBuff.buff, texture.image, VK_IMAGE_LAYOUT_GENERAL, 1, &region);
	EndTemporaryCmdBuffer(test->device, test->cmdPool, test->queue, tmp);
	test->textures.push_back(texture);

	test->textures_h.Add(new RenderTexture(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, globals));
}

int AddTextureFromPath(const char *path)
{
	int texWidth, texHeight, texChannels;
    unsigned char *data = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (!data)
		return 0;
	AddTextureFromData(texWidth, texHeight, data);
	free(data);
	return 1;
}

void ScreenShot(uint32_t index, uint32_t width, uint32_t height)
{
	Buffer tmpBuff = Buffer(globals, test->storageBuffers[index].size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkCommandBuffer tmp = BeginTemporaryCmdBuffer(test->device, test->cmdPool);
		VkBufferCopy region = {};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = test->storageBuffers[index].size;
		vkCmdCopyBuffer(tmp, test->storageBuffers[index].buff, tmpBuff.buff, 1, &region);
	EndTemporaryCmdBuffer(test->device, test->cmdPool, test->queue, tmp);

	tmpBuff.Map();
		float *raw = (float*)tmpBuff.GetData();
		uint8_t *pixels = new uint8_t[800 * 800 * 4];
		uint32_t gI = 0;
		for (uint32_t i = 0; i < width; i++)
			for (uint32_t j = 0; j < height; j++)
				for (uint32_t k = 0; k < 4; k++)
				{
					uint32_t tt = j * 4 + (width - 1 - i) * width * 4 + k;
					raw[tt] = pow(raw[tt], 1.0 / 2.2);
					uint8_t c = ((raw[tt] > 1.0) ? 255 : raw[tt] * 255);
					pixels[gI] = c;
					gI++;
				}
	tmpBuff.Unmap();

	stbi_write_png("./t.png", 800, 800, 4, pixels, 0);
	delete[] pixels;
}

void AddComputePipe(const char *shader)
{
	ComputePipeline tmp = ComputePipeline(shader, globals, test->computeDescriptorSetLayouts, test->computeDescriptorSets);
	test->computePipelines.push_back(tmp);
}

void SetUpGraphicsPipe(const char *vertexShader, const char *fragmentShader)
{
	CreateGraphicsDescriptors();
	CreateGraphicsPipe(vertexShader, fragmentShader);
	CreateFrameBuffers();
	CreateGraphicsCommandBuffers();
	CreateSemaphores();
}

void MapMemory()
{
	for (uint32_t i = 0; i < test->uniformBuffers.size(); ++i)
		test->uniformBuffers[i].Map();
}

void UnmapMemory()
{
	for (uint32_t i = 0; i < test->uniformBuffers.size(); ++i)
		test->uniformBuffers[i].Unmap();
}


void DispatchComputeShader(uint32_t index, bool sync)
{
	test->computePipelines[index].Dispatch(test->queue, true);
}

void DrawFrame()
{
	uint32_t imageIndex;
    vkAcquireNextImageKHR(test->device, test->swapchain, UINT64_MAX, test->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &test->imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &test->commandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &test->renderFinishedSemaphore;
	vkQueueSubmit(test->queue, 1, &submitInfo, test->fence);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &test->renderFinishedSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &test->swapchain;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;
	vkQueuePresentKHR(test->queue, &presentInfo);
}

void UpdateBuffer(uint32_t index, void *data)
{
	test->uniformBuffers[index].SetData(data);
}

#include <stdarg.h>
void SetWindowTitle(const char *s, ...)
{
	char *c = new char[999999];

    va_list argptr;
    va_start(argptr, s);
    	vsprintf(c, s, argptr);
		glfwSetWindowTitle(test->window, c);
    va_end(argptr);
	delete[] c;
}

void Sync()
{
	vkWaitForFences(test->device, 1, &test->fence, VK_TRUE, INT32_MAX);
	vkResetFences(test->device, 1, &test->fence);
}
