#pragma once

#include "ISampler.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"

class VulkanSampler : public ISampler
{
public:
    void Create(IDevice* device, SamplerDesc desc) override;
    void Destroy() override;

    VkSampler GetSampler() const { return textureSampler; }

private:
    VkSampler     textureSampler = VK_NULL_HANDLE;
    VulkanDevice* vkDevice       = nullptr;
};
