
//-----
// Pub
//-----

void Initter::SetWindowTitle(const char *s, ...)
{
	char *c = new char[999999]; // Fix this

    va_list argptr;
    va_start(argptr, s);
    	vsprintf(c, s, argptr);
		glfwSetWindowTitle(screen.window, c);
    va_end(argptr);
	delete[] c;
}

//-----
// Priv
//-----
void Initter::InitInstance()
{
    uint32_t extensionCount;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app.pApplicationName = "Test";
    app.applicationVersion = 0;
    app.pEngineName = "Test";
    app.engineVersion = 0;
    app.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &app;
    instanceInfo.enabledExtensionCount = extensionCount;
    instanceInfo.ppEnabledExtensionNames = extensions;
    vkCreateInstance(&instanceInfo, nullptr, &instance);
}

void Initter::InitGpu()
{
	uint32_t gpuCount;
    vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
	VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[gpuCount];
    vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices);
	gpu = physicalDevices[0];
    delete[] physicalDevices;
}

void Initter::InitDevice()
{
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueCount, nullptr);
	VkQueueFamilyProperties *queueProps = new VkQueueFamilyProperties[queueCount];
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueCount, queueProps);

    uint32_t graphicsQueueFamilyIndex = 0;
    for (size_t i = 0; i < queueCount; i++)
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamilyIndex = i; break; }
    
    const float defaultQueuePriority = 0.0;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &defaultQueuePriority;

    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    VkPhysicalDeviceFeatures features = {};
    deviceInfo.pEnabledFeatures = &features;

    vkCreateDevice(gpu, &deviceInfo, nullptr, &device);
    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);
}

void Initter::InitCmdPool()
{
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = 0;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool);
}

void Initter::InitSurface()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    screen.window = glfwCreateWindow(screen.width, screen.height, "Test", nullptr, nullptr);
    glfwCreateWindowSurface(instance, screen.window, nullptr, &screen.surface);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, screen.surface, &formatCount, nullptr);
	VkSurfaceFormatKHR *surfFormats = new VkSurfaceFormatKHR[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, screen.surface, &formatCount, surfFormats);

    screen.format = surfFormats[2].format;
    screen.colorSpace = surfFormats[2].colorSpace;
	delete[] surfFormats;
}
