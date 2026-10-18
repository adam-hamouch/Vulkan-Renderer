#pragma once

#include "IMaterial.h"
#include "VulkanUtils.h"

class VulkanMaterial : public IMaterial
{
public:
    void Create(IDevice* device, MaterialDesc desc) override;
    void Destroy() override;

    void Bind(ICommandList* cmdList) override;

private:
    VkDevice                     device         = VK_NULL_HANDLE;
    VkDescriptorPool             descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;
};
