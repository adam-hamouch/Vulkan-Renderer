#include "../Include/RHI/VulkanSwapChain.h"
#include "../Include/RHI/VulkanSurface.h"
#include "../Include/RHI/VulkanQueue.h"
#include "Core/Window/Window.h"

void VulkanSwapChain::Create(IDevice* device, ISurface* surface, Window* window)
{
    VulkanSurface* vkSurface = static_cast<VulkanSurface*>(surface);

    vkDevice      = static_cast<VulkanDevice*>(device);
    this->surface = surface;
    this->window  = window;
    
    SwapChainSupportDetails swapChainSupport = vkDevice->QuerySwapChainSupport(vkDevice->GetPhysicalDevice());

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);
        
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vkSurface->GetVkSurfaceKHR();
        
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
    
        QueueFamilyIndices indices = vkDevice->FindQueueFamilies(vkDevice->GetPhysicalDevice());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; 
            createInfo.pQueueFamilyIndices = nullptr; 
        }
        
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        
        if (vkCreateSwapchainKHR(vkDevice->GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création de la swap chain!");
        }
        
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    
        createImageViews();
        createSyncObjects();
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return availableFormat;
        }
    }
        
    return availableFormats[0];
}
    
VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) 
{
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
        
    return VK_PRESENT_MODE_FIFO_KHR;
}
    
VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window) 
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } 
    else 
    {
        int width, height;
        glfwGetFramebufferSize(window->GetHandle(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);

        actualExtent.height = std::clamp(actualExtent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void VulkanSwapChain::createImageViews() 
{
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(vkDevice->GetDevice(), swapChain, &imageCount, nullptr);
    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(vkDevice->GetDevice(), swapChain, &imageCount, images.data());

    swapChainImages.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++) {
        VulkanImage& image = swapChainImages[i];
        image.vkDevice  = vkDevice;
        image.image     = images[i];
        image.format    = swapChainImageFormat;
        image.ownsImage = false;
        image.createImageView();
    }
}

void VulkanSwapChain::createSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    currentFrame = 0;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(vkDevice->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vkDevice->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vkDevice->GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("échec de la création des objets de synchronisation pour une frame!");
        }
    }
}

void VulkanSwapChain::Destroy()
{
    for (size_t i = 0; i < inFlightFences.size(); i++) {
        vkDestroySemaphore(vkDevice->GetDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vkDevice->GetDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vkDevice->GetDevice(), inFlightFences[i], nullptr);
    }
    renderFinishedSemaphores.clear();
    imageAvailableSemaphores.clear();
    inFlightFences.clear();

    for (VulkanImage& image : swapChainImages)
        image.Destroy();
    swapChainImages.clear();

    vkDestroySwapchainKHR(vkDevice->GetDevice(), swapChain, nullptr);
    swapChain = VK_NULL_HANDLE;
}

void VulkanSwapChain::Resize(uint32_t width, uint32_t height)
{
    Destroy();
    Create(vkDevice, surface, window);
}

void VulkanSwapChain::Present(IQueue* queue)
{
    VkSemaphore waitSemaphores[] = {renderFinishedSemaphores[currentFrame]};

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = waitSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;

    VkResult result = vkQueuePresentKHR(static_cast<VulkanQueue*>(queue)->GetQueue(), &presentInfo);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR && result != VK_ERROR_OUT_OF_DATE_KHR) {
        throw std::runtime_error("échec de la présentation d'une image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

uint32_t VulkanSwapChain::AcquireNextImage()
{
    vkWaitForFences(vkDevice->GetDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(vkDevice->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("échec de l'acquisition d'une image de la swap chain!");
    }

    vkResetFences(vkDevice->GetDevice(), 1, &inFlightFences[currentFrame]);

    return imageIndex;
}
