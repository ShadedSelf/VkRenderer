
//-----
// Pub
//-----

void Drawer::DrawFrame()
{
	uint32_t imageIndex;
    vkAcquireNextImageKHR(init->device, swapchain, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailable;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderingDone;
	vkQueueSubmit(init->queue, 1, &submitInfo, fence);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderingDone;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;
	vkQueuePresentKHR(init->queue, &presentInfo);

	vkWaitForFences(init->device, 1, &fence, VK_TRUE, UINT64_MAX);
	vkResetFences(init->device, 1, &fence);
}

//-----
// Priv
//-----

void Drawer::CreateSwapchain()
{
	VkBool32 surfaceSupport;
	vkGetPhysicalDeviceSurfaceSupportKHR(init->gpu, 0, init->screen.surface, &surfaceSupport);
    VkSurfaceCapabilitiesKHR surfCaps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(init->gpu, init->screen.surface, &surfCaps);

    uint32_t minImageCount = surfCaps.minImageCount;

    VkSwapchainCreateInfoKHR swapchainInfo = {};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = init->screen.surface;
    swapchainInfo.minImageCount = minImageCount;
    swapchainInfo.imageFormat = init->screen.format;
    swapchainInfo.imageColorSpace = init->screen.colorSpace;
    swapchainInfo.imageExtent = surfCaps.currentExtent;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = surfCaps.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.queueFamilyIndexCount = 0;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainInfo.clipped = VK_TRUE;
    vkCreateSwapchainKHR(init->device, &swapchainInfo, nullptr, &swapchain);

    vkGetSwapchainImagesKHR(init->device, swapchain, &swapchainImageCount, nullptr);
    VkImage *swapchainImages = new VkImage[swapchainImageCount];
    vkGetSwapchainImagesKHR(init->device, swapchain, &swapchainImageCount, swapchainImages);
    swapchainBuffers = new SwapchainBuffer[swapchainImageCount];

    for (int i = 0; i < swapchainImageCount; i++)
	{
        VkImageViewCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imageInfo.format = init->screen.format;
        imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
       	imageInfo.subresourceRange.baseMipLevel = 0;
        imageInfo.subresourceRange.levelCount = 1;
        imageInfo.subresourceRange.baseArrayLayer = 0;
        imageInfo.subresourceRange.layerCount = 1;
        imageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageInfo.image = swapchainImages[i];

        swapchainBuffers[i].image = swapchainImages[i];
        vkCreateImageView(init->device, &imageInfo, nullptr, &swapchainBuffers[i].view);
    }
    delete[] swapchainImages;
}

void Drawer::CreateSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(init->device, &semaphoreInfo, nullptr, &imageAvailable);
	vkCreateSemaphore(init->device, &semaphoreInfo, nullptr, &renderingDone);

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	vkCreateFence(init->device, &fenceInfo, nullptr, &fence);
	vkResetFences(init->device, 1, &fence);
}
