#pragma once

#include "IDevice.h"
#include "VulkanUtils.h"
#include <optional>
#include <set>
#include <vector>

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanDevice : public IDevice
{
public:
    void Create(IInstance* instance, ISurface* surface) override;
    void Destroy() override;
    
    VkPhysicalDevice      GetPhysicalDevice() const { return physicalDevice; }
    VkDevice              GetDevice()         const { return device; }
    VkQueue               GetGraphicsQueue()  const { return graphicsQueue; }
    VkQueue               GetPresentQueue()   const { return presentQueue; }
    VkSampleCountFlagBits GetMsaaSamples()    const { return msaaSamples; }
    VkSurfaceKHR          GetSurface()        const { return surface; }
    
    QueueFamilyIndices      FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

private:
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    
    VkPhysicalDevice      physicalDevice = VK_NULL_HANDLE;
    VkDevice              device         = VK_NULL_HANDLE;
    VkQueue               graphicsQueue  = VK_NULL_HANDLE;
    VkQueue               presentQueue   = VK_NULL_HANDLE;
    VkSurfaceKHR          surface        = VK_NULL_HANDLE;
    VkInstance          instance        = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaaSamples    = VK_SAMPLE_COUNT_1_BIT;
    
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};