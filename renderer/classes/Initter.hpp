#ifndef INITTER
    #define INITTER

#include <vulkan/vulkan.h>

struct Screen
{
    GLFWwindow *window;
    VkSurfaceKHR surface;
    VkFormat format;
    VkColorSpaceKHR colorSpace;

    uint32_t width;
    uint32_t height;
};

class Initter
{
    private:
        void InitInstance();
        void InitGpu();
        void InitDevice();
        void InitSurface();
        void InitCmdPool();

    public:
        VkInstance instance;
        VkPhysicalDevice gpu;
        VkDevice device;
        VkQueue queue;
        VkCommandPool cmdPool;

        Screen screen;

        Initter(uint32_t width, uint32_t height)
        {
            screen.width = width;
            screen.height = height;

            InitInstance();
            InitGpu();
            InitDevice();
            InitCmdPool();
            InitSurface();
        }
		~Initter()
		{
			vkDestroyCommandPool(device, cmdPool, nullptr);
			vkDestroySurfaceKHR(instance, screen.surface, nullptr);
			vkDestroyDevice(device, nullptr);
			vkDestroyInstance(instance, nullptr);
		}

		void SetWindowTitle(const char *s, ...);
};

#include <Initter.cpp>

#endif
