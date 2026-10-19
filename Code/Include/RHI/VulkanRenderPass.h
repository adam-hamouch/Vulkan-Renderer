#pragma once

#include "IRenderPass.h"
#include "../Include/RHI/VulkanUtils.h"
#include "../Include/RHI/VulkanDevice.h"

class VulkanRenderPass : public IRenderPass
{
public:
    void Create(IDevice* device, RenderPassDesc desc) override;
    void Destroy() override;

    void Begin(ICommandList* commandist, IFramebuffer* framebuffer, uint32_t imageIndex) override;
    void End(ICommandList* commandist) override;
    
    VkRenderPass Get() {return renderPass;}
    const std::vector<VkAttachmentDescription>& GetAttachments() const {return attachments;}
    const std::vector<Attachment>& GetAttachmentDescs() const {return attachmentDescs;}

private:
    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkRenderPass renderPass;
    std::vector<VkAttachmentDescription> attachments;
    std::vector<Attachment> attachmentDescs;
    VulkanDevice* vkDevice;
};
