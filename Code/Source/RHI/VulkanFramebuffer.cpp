#include "../Include/RHI/VulkanFramebuffer.h"
#include "../Include/RHI/VulkanSwapChain.h"
#include "../Include/RHI/VulkanRenderPass.h"

void VulkanFramebuffer::Create(IDevice* device, ISwapChain* swapChain, IRenderPass* renderPass)
{
    vkDevice = static_cast<VulkanDevice*>(device);
    vkSwapChain = static_cast<VulkanSwapChain*>(swapChain);
    vkRenderPass = static_cast<VulkanRenderPass*>(renderPass);

    extent = vkSwapChain->GetExtent();

    createAttachments();
    createFramebuffers();
}

void VulkanFramebuffer::Destroy()
{
    if (!vkDevice)
        return;

    VkDevice device = vkDevice->GetDevice();

    for (VkFramebuffer framebuffer : framebuffers)
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    framebuffers.clear();

    for (AttachmentImage& attachment : attachmentImages) {
        if (attachment.image == VK_NULL_HANDLE)
            continue;

        vkDestroyImageView(device, attachment.view, nullptr);
        vkDestroyImage(device, attachment.image, nullptr);
        vkFreeMemory(device, attachment.memory, nullptr);
    }
    attachmentImages.clear();
}

void VulkanFramebuffer::Recreate(ISwapChain* swapChain)
{
    Destroy();
    Create(vkDevice, swapChain, vkRenderPass);
}

void VulkanFramebuffer::createAttachments()
{
    const std::vector<VkAttachmentDescription>& descriptions = vkRenderPass->GetAttachments();
    attachmentImages.resize(descriptions.size());

    for (size_t i = 0; i < descriptions.size(); i++) {
        const VkAttachmentDescription& desc = descriptions[i];

        if (desc.finalLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
            continue;

        bool isDepth = desc.finalLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkImageUsageFlags  usage  = isDepth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                            : VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        VkImageAspectFlags aspect = isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

        AttachmentImage& attachment = attachmentImages[i];
        createImage(extent.width, extent.height, desc.samples, desc.format, usage, attachment.image, attachment.memory);
        attachment.view = vkSwapChain->createImageView(attachment.image, desc.format, aspect, 1);
    }
}

void VulkanFramebuffer::createFramebuffers()
{
    const std::vector<VkImageView>& swapChainImageViews = vkSwapChain->GetImageViews();
    framebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::vector<VkImageView> attachments(attachmentImages.size());
        for (size_t a = 0; a < attachmentImages.size(); a++) {
            attachments[a] = attachmentImages[a].image != VK_NULL_HANDLE ? attachmentImages[a].view
                                                                         : swapChainImageViews[i];
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkRenderPass->Get();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vkDevice->GetDevice(), &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création d'un framebuffer!");
        }
    }
}

void VulkanFramebuffer::createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageUsageFlags usage, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(vkDevice->GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'une image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkDevice->GetDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vkDevice->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(vkDevice->GetDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de la memoire d'une image!");
    }

    vkBindImageMemory(vkDevice->GetDevice(), image, imageMemory, 0);
}
