#pragma once

#include "IFramebuffer.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"

class VulkanSwapChain;
class VulkanRenderPass;

class VulkanFramebuffer : public IFramebuffer
{
public:
    void Create(IDevice* device, ISwapChain* swapChain, IRenderPass* renderPass) override;
    void Destroy() override;

    void Recreate(ISwapChain* swapChain) override;

    VkFramebuffer GetFramebuffer(uint32_t imageIndex) const { return framebuffers[imageIndex]; }
    VkExtent2D    GetExtent()                         const { return extent; }

private:
    struct AttachmentImage
    {
        VkImage        image  = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView    view   = VK_NULL_HANDLE;
    };

    void createAttachments();
    void createFramebuffers();
    void createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageUsageFlags usage, VkImage& image, VkDeviceMemory& imageMemory);

    VulkanDevice*     vkDevice     = nullptr;
    VulkanSwapChain*  vkSwapChain  = nullptr;
    VulkanRenderPass* vkRenderPass = nullptr;

    VkExtent2D extent{};

    std::vector<AttachmentImage> attachmentImages;
    std::vector<VkFramebuffer>   framebuffers;
};
