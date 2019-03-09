#define GLFW_INCLUDE_NONE
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>

#include <Initter.hpp>
#include <Drawer.hpp>

#include <BindableBuffers.hpp>
#include <GraphicsPipeline.hpp>
#include <ComputePipeline.hpp>

// #include <Renderer.hpp>

int main()
{
    glfwInit();
    Initter init = Initter(800, 800);
    Drawer drawer = Drawer(&init);

	BindableBuffers globalBuffers = BindableBuffers(&init);
	globalBuffers.AddBuffer("Time", sizeof(float), UniformBuffer, 0, true);
    globalBuffers.AddBuffer("Test", sizeof(float), UniformBuffer, 1, true);
    RenderTexture tex = RenderTexture(&init, 800, 800, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT);
	globalBuffers.AddTexture("RenderStorage", StorageTexture, 2, tex);
	globalBuffers.AddTexture("RenderSampler", SamplerTexture, 3, tex);

    GraphicsPipeline graphicsPipe = GraphicsPipeline(&init);
    graphicsPipe.BindBuffers(&globalBuffers);
    graphicsPipe.Create(drawer.swapchainImageCount, drawer.swapchainBuffers, "obs/vert.spv", "obs/frag.spv");

    ComputePipeline computePipe = ComputePipeline(&init);
    computePipe.BindBuffers(&globalBuffers);
    computePipe.Create("obs/test.spv");
     
    drawer.SetPipeCmdBuffers(graphicsPipe.cmdBuffers);
    while (!glfwGetKey(init.screen.window, GLFW_KEY_ESCAPE))
    {
        glfwPollEvents();
        computePipe.Dispatch(false);
        drawer.DrawFrame();
    }
}
