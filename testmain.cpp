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
    globalBuffers.AddUniformBuffer("Time", sizeof(float), 0, 0);
    float aaa = 0.5;
    globalBuffers.GetUniformBuffer("Time")->SetData(&aaa);
    // RenderTexture render = RenderTexture(&init, 800, 800, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT);

    // BindableBuffers testBuffers = globalBuffers;
    // testBuffers.AddTexture(render);
    // GraphicsPipeline graphicsPipe = GraphicsPipeline(&init);
    // graphicsPipe.BindBuffers(&testBuffers);
    // graphicsPipe.Create(drawer.swapchainImageCount, drawer.swapchainBuffers, "obs/vert.spv", "obs/frag.spv");

    // BindableBuffers computeBuffers = globalBuffers;
    // computeBuffers.AddRenderTexture(render);
    // ComputePipeline computePipe = ComputePipeline(&init);
    // computePipe.BindBuffers(&computeBuffers);
    // computePipe.Create("obs/test.spv");

    GraphicsPipeline graphicsPipe = GraphicsPipeline(&init);
    graphicsPipe.BindBuffers(&globalBuffers);
    graphicsPipe.Create(drawer.swapchainImageCount, drawer.swapchainBuffers, "obs/vert.spv", "obs/frag.spv");
     
    drawer.SetPipeCmdBuffers(graphicsPipe.cmdBuffers);
    while (!glfwGetKey(init.screen.window, GLFW_KEY_ESCAPE))
    {
        glfwPollEvents();
        // computePipe.Dispatch(false);
        drawer.DrawFrame();
    }
}
