#pragma once

#include "VulkanUtils.h"
#include "VulkanDevice.h"

#include "ITexture.h"
#include <cstdint>

class VulkanTexture : public ITexture
{
public:
    void Load(const char* path, IDevice* device, ISwapChain* swapChain) override;
    void Destroy() override;

    VkImageView GetImageView() const { return textureImageView; }

private :
    void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    uint32_t mipLevels = 1;
    VkImageView textureImageView;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VulkanDevice* vkDevice;
};
