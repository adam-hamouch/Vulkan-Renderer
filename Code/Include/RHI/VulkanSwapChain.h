#pragma once

#include "ISwapChain.h"
#include "../Include/RHI/VulkanDevice.h"
#include "../Include/RHI/VulkanUtils.h"

class VulkanSwapChain : public ISwapChain
{
public:
    void Create(IDevice* device, ISurface* surface, Window* window) override;
    void Destroy() override;

    void Resize(uint32_t width, uint32_t height) override;
    void Present(IQueue* queue) override;
    uint32_t AcquireNextImage() override;
    
    VkFormat GetSwapChainImageFormat() {return swapChainImageFormat;}
    
private:
    
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    void createImageViews(); 
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VulkanDevice* vkDevice;
};
