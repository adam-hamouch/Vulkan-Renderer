#include "RHI/VulkanImage.h"

static VkFormat ToVkFormat(Format format)
{
    switch (format)
    {
    case Format::R8_UNORM:          return VK_FORMAT_R8_UNORM;
    case Format::RGBA8_UNORM:       return VK_FORMAT_R8G8B8A8_UNORM;
    case Format::RGBA8_SRGB:        return VK_FORMAT_R8G8B8A8_SRGB;
    case Format::BGRA8_UNORM:       return VK_FORMAT_B8G8R8A8_UNORM;
    case Format::BGRA8_SRGB:        return VK_FORMAT_B8G8R8A8_SRGB;
    case Format::RGBA16_FLOAT:      return VK_FORMAT_R16G16B16A16_SFLOAT;
    case Format::D32_FLOAT:         return VK_FORMAT_D32_SFLOAT;
    case Format::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
    default:                        return VK_FORMAT_UNDEFINED;
    }
}

static VkImageUsageFlags ToVkUsage(Usage usage)
{
    VkImageUsageFlags flags = 0;
    if (usage & Usage::Sampled)         flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (usage & Usage::Storage)         flags |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (usage & Usage::ColorAttachment) flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (usage & Usage::DepthAttachment) flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (usage & Usage::TransferSrc)     flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (usage & Usage::TransferDst)     flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    return flags;
}

void VulkanImage::Create(IDevice* device, const ImageDesc& desc)
{
    vkDevice    = static_cast<VulkanDevice*>(device);
    format      = ToVkFormat(desc.format);
    mipLevels   = desc.mipLevels;
    ownsImage   = true;
    aspectFlags = (desc.format == Format::D32_FLOAT || desc.format == Format::D24_UNORM_S8_UINT) ? VK_IMAGE_ASPECT_DEPTH_BIT
                                                                                                  : VK_IMAGE_ASPECT_COLOR_BIT;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = desc.width;
    imageInfo.extent.height = desc.height;
    imageInfo.extent.depth = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = ToVkUsage(desc.usage);
    imageInfo.samples = static_cast<VkSampleCountFlagBits>(desc.sampleCount);
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.mipLevels = mipLevels;

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

    createImageView();
}

void VulkanImage::createImageView()
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(vkDevice->GetDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("échec de la creation de la vue sur une image!");
    }
}

void VulkanImage::Destroy()
{
    if (!vkDevice)
        return;

    if (imageView != VK_NULL_HANDLE)
        vkDestroyImageView(vkDevice->GetDevice(), imageView, nullptr);

    if (ownsImage) {
        vkDestroyImage(vkDevice->GetDevice(), image, nullptr);
        vkFreeMemory(vkDevice->GetDevice(), imageMemory, nullptr);
    }

    image       = VK_NULL_HANDLE;
    imageMemory = VK_NULL_HANDLE;
    imageView   = VK_NULL_HANDLE;
    ownsImage   = false;
}
