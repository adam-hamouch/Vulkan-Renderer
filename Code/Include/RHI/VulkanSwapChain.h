#pragma once

#include "ISwapChain.h"
#include "../Include/RHI/VulkanDevice.h"
#include "../Include/RHI/VulkanUtils.h"
#include "../Include/RHI/VulkanImage.h"

class VulkanSwapChain : public ISwapChain
{
public:
    void Create(IDevice* device, ISurface* surface, Window* window) override;
    void Destroy() override;

    void Resize(uint32_t width, uint32_t height) override;
    void Present(IQueue* queue) override;
    uint32_t AcquireNextImage() override;
    uint32_t GetCurrentFrame() const override { return currentFrame; }
    
    VkFormat GetSwapChainImageFormat() {return swapChainImageFormat;}
    VkExtent2D GetExtent() const {return swapChainExtent;}
    const std::vector<VulkanImage>& GetImages() const {return swapChainImages;}

    VkSemaphore GetImageAvailableSemaphore() const { return imageAvailableSemaphores[currentFrame]; }
    VkSemaphore GetRenderFinishedSemaphore() const { return renderFinishedSemaphores[currentFrame]; }
    VkFence     GetInFlightFence()           const { return inFlightFences[currentFrame]; }
    
private:
    
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    void createImageViews(); 
    void createSyncObjects();
    
    VkSwapchainKHR swapChain;
    std::vector<VulkanImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VulkanDevice* vkDevice;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
    uint32_t currentFrame = 0;
    uint32_t imageIndex   = 0;

    ISurface* surface = nullptr;
    Window*   window  = nullptr;
};
