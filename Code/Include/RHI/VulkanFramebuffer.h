#pragma once

#include "IFramebuffer.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

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
    void createAttachments();
    void createFramebuffers();

    VulkanDevice*     vkDevice     = nullptr;
    VulkanSwapChain*  vkSwapChain  = nullptr;
    VulkanRenderPass* vkRenderPass = nullptr;

    VkExtent2D extent{};

    std::vector<VulkanImage>     attachmentImages;
    std::vector<VkFramebuffer>   framebuffers;
};
