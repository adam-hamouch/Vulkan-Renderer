#pragma once

#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

#include "ITexture.h"
#include <cstdint>

class VulkanTexture : public ITexture
{
public:
    void Load(const char* path, IDevice* device) override;
    void Destroy() override;

    VkImageView GetImageView() const { return image.GetImageView(); }

private :
    void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    uint32_t mipLevels = 1;
    VulkanImage image;
    VulkanDevice* vkDevice = nullptr;
};
