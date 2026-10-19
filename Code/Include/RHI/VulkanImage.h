#pragma once

#include <cstdint>
#include "IImage.h"
#include "RHI/VulkanUtils.h"
#include "RHI/IDevice.h"
#include "RHI/VulkanDevice.h"

class VulkanImage : public IImage
{
	friend class VulkanSwapChain;

public:
	void Create(IDevice* device, const ImageDesc& desc) override;
	void Destroy() override;

	VkImage     GetImage()     const { return image; }
	VkImageView GetImageView() const { return imageView; }

private:
	void createImageView();

	VulkanDevice* vkDevice = nullptr;

	VkImage            image       = VK_NULL_HANDLE;
	VkDeviceMemory     imageMemory = VK_NULL_HANDLE;
	VkImageView        imageView   = VK_NULL_HANDLE;
	VkFormat           format      = VK_FORMAT_UNDEFINED;
	VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	uint32_t           mipLevels   = 1;
	bool               ownsImage   = false;
};
