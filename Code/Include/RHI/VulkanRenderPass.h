#pragma once

#include "IRenderPass.h"
#include "../Include/RHI/VulkanUtils.h"
#include "../Include/RHI/VulkanDevice.h"

class VulkanRenderPass : public IRenderPass
{
public:
    void Create(IDevice* device, RenderPassDesc desc) override;
    void Destroy() override;

    void Begin(ICommandList* commandist) override;
    void End(ICommandList* commandist) override;
    
    VkRenderPass Get() {return renderPass;}
    
private:
    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    
    VkRenderPass renderPass;
    VulkanDevice* vkDevice;
};
