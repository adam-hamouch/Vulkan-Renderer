#include "../Include/RHI/VulkanRenderPass.h"
#include "../Include/RHI/VulkanSwapChain.h"

void VulkanRenderPass::Create(IDevice* device, RenderPassDesc desc)
{
    VulkanDevice* vkDevice = static_cast<VulkanDevice*>(device);

    std::vector<VkAttachmentDescription> attachments;
    for (const auto& att : desc.attachments)
    {
        VkAttachmentDescription vkAtt{};
        
        switch (att.format) {
            case AttachmentFormat::BGRA8:           vkAtt.format = VK_FORMAT_B8G8R8A8_SRGB; break;
            case AttachmentFormat::RGBA8:           vkAtt.format = VK_FORMAT_R8G8B8A8_SRGB; break;
            case AttachmentFormat::Depth32:         vkAtt.format = VK_FORMAT_D32_SFLOAT;    break;
            case AttachmentFormat::Depth24Stencil8: vkAtt.format = VK_FORMAT_D24_UNORM_S8_UINT; break;
        }

        switch (att.sampleCount) {
            case 1:  vkAtt.samples = VK_SAMPLE_COUNT_1_BIT;  break;
            case 4:  vkAtt.samples = VK_SAMPLE_COUNT_4_BIT;  break;
            case 8:  vkAtt.samples = VK_SAMPLE_COUNT_8_BIT;  break;
            default: vkAtt.samples = VK_SAMPLE_COUNT_1_BIT;  break;
        }

        switch (att.loadOp) {
            case LoadOp::CLEAR:   vkAtt.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;    break;
            case LoadOp::LOAD:    vkAtt.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;     break;
            case LoadOp::DONTCARE:vkAtt.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;break;
        }

        switch (att.storeOp) {
            case StoreOp::STORE:   vkAtt.storeOp = VK_ATTACHMENT_STORE_OP_STORE;    break;
            case StoreOp::DONTCARE:vkAtt.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;break;
        }

        vkAtt.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        vkAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        if (att.isDepth) {
            vkAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkAtt.finalLayout   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        } else if (att.isResolve) {
            vkAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkAtt.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        } else {
            vkAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkAtt.finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        attachments.push_back(vkAtt);
    }

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthRef{};
    depthRef.attachment = 1;
    depthRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference resolveRef{};
    resolveRef.attachment = 2;
    resolveRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorRef;
    subpass.pDepthStencilAttachment = &depthRef;
    subpass.pResolveAttachments     = &resolveRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass    = 0;
    dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                               VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                               VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments    = attachments.data();
    renderPassInfo.subpassCount    = 1;
    renderPassInfo.pSubpasses      = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies   = &dependency;

    if (vkCreateRenderPass(vkDevice->GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        throw std::runtime_error("échec de la création de la render pass!");
}

VkFormat VulkanRenderPass::findDepthFormat() 
{
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat VulkanRenderPass::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) 
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vkDevice->GetPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("aucun des formats demandés n'est supporté!");
}


void VulkanRenderPass::Destroy()
{
}

void VulkanRenderPass::Begin(ICommandList* commandist)
{
}

void VulkanRenderPass::End(ICommandList* commandist)
{
}
