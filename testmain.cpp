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
#include <Camera.hpp>

struct Globals
{
	float time;
	uint32_t frame;
	float deltaTime;
	uint32_t pad0;
};

int main()
{
    glfwInit();
    Initter init = Initter(800, 800);
    Drawer drawer = Drawer(&init);

	RenderTexture tex = RenderTexture(&init, 800, 800, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT);
	
	BindableBuffers graphicsBuffers = BindableBuffers(&init);
	graphicsBuffers.AddTexture("RenderSampler", SamplerTexture, 0, tex);

	BindableBuffers computeBuffers = BindableBuffers(&init);
	computeBuffers.AddTexture("RenderStorage", 							  StorageTexture, 0, tex);
	computeBuffers.AddBuffer("Globals", sizeof(float) * 4, 				   UniformBuffer, 1, true, false);
	computeBuffers.AddBuffer("Cam", 	sizeof(float) * 4 * 4, 			   UniformBuffer, 2, true, false);
	computeBuffers.AddBuffer("Accum", 	sizeof(float) * 4 * 1 * 800 * 800, StorageBuffer, 3, false, true);
	computeBuffers.AddBuffer("Bounces", sizeof(float) * 4 * 4 * 800 * 800, StorageBuffer, 4, false, true);
	computeBuffers.AddBuffer("Rays", 	sizeof(float) * 4 * 2 * 800 * 800, StorageBuffer, 5, false, true);

    GraphicsPipeline graphicsPipe = GraphicsPipeline(&init);
    graphicsPipe.BindBuffers(&graphicsBuffers);
    graphicsPipe.Create(drawer.swapchainImageCount, drawer.swapchainBuffers, "obs/vert.spv", "obs/frag.spv");

    ComputePipeline setPipe = ComputePipeline(&init);
    setPipe.BindBuffers(&computeBuffers);
    setPipe.Create("obs/set.spv");

	ComputePipeline computePipe = ComputePipeline(&init);
    computePipe.BindBuffers(&computeBuffers);
    computePipe.Create("obs/comp.spv");
    
	Camera cam = Camera(&init, float3(-1, 2, -5));

    drawer.SetPipeCmdBuffers(graphicsPipe.cmdBuffers);
	Globals globals = {};
    while (!glfwGetKey(init.screen.window, GLFW_KEY_ESCAPE))
    {
        glfwPollEvents();
		init.SetWindowTitle("asd");
		
		if (globals.frame == 0 || cam.Update(0.1, 0.5, 0.1))
		{
			CamData camData = cam.Get();
			computeBuffers.GetBuffer("Cam")->SetData(&camData);
			setPipe.Dispatch(true);
		}
		for (uint32_t i = 0; i < 2; i++)
		{
			computeBuffers.GetBuffer("Globals")->SetData(&globals);
			computePipe.Dispatch(true);
			globals.frame++;
		}
        drawer.DrawFrame();
    }
}
