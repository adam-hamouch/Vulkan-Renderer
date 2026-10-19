#include "../Include/RHI/VulkanFramebuffer.h"
#include "../Include/RHI/VulkanSwapChain.h"
#include "../Include/RHI/VulkanRenderPass.h"

static Format ToFormat(AttachmentFormat format)
{
    switch (format)
    {
    case AttachmentFormat::RGBA8:           return Format::RGBA8_SRGB;
    case AttachmentFormat::BGRA8:           return Format::BGRA8_SRGB;
    case AttachmentFormat::Depth32:         return Format::D32_FLOAT;
    case AttachmentFormat::Depth24Stencil8: return Format::D24_UNORM_S8_UINT;
    default:                                return Format::Undefined;
    }
}

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

    for (VulkanImage& attachment : attachmentImages)
        attachment.Destroy();
    attachmentImages.clear();
}

void VulkanFramebuffer::Recreate(ISwapChain* swapChain)
{
    Destroy();
    Create(vkDevice, swapChain, vkRenderPass);
}

void VulkanFramebuffer::createAttachments()
{
    const std::vector<Attachment>& descriptions = vkRenderPass->GetAttachmentDescs();
    attachmentImages.resize(descriptions.size());

    for (size_t i = 0; i < descriptions.size(); i++) {
        const Attachment& att = descriptions[i];

        if (att.isResolve)
            continue;

        ImageDesc desc;
        desc.width       = extent.width;
        desc.height      = extent.height;
        desc.sampleCount = static_cast<uint32_t>(att.sampleCount);
        desc.format      = ToFormat(att.format);
        desc.usage       = att.isDepth ? Usage::DepthAttachment : Usage::ColorAttachment;

        attachmentImages[i].Create(vkDevice, desc);
    }
}

void VulkanFramebuffer::createFramebuffers()
{
    const std::vector<VulkanImage>& swapChainImages = vkSwapChain->GetImages();
    framebuffers.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        std::vector<VkImageView> attachments(attachmentImages.size());
        for (size_t a = 0; a < attachmentImages.size(); a++) {
            attachments[a] = attachmentImages[a].GetImage() != VK_NULL_HANDLE ? attachmentImages[a].GetImageView()
                                                                              : swapChainImages[i].GetImageView();
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
