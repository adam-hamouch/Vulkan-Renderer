#pragma once

#include "IPipeline.h"
#include "VulkanUtils.h"

class VulkanPipeline : public IPipeline
{
public:
    void Create(IDevice* device, PipelineDesc desc) override;
    void Destroy() override;

    VkPipeline            GetPipeline()            const { return graphicsPipeline; }
    VkPipelineLayout      GetPipelineLayout()      const { return pipelineLayout; }
    VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    void createDescriptorSetLayout();

    static VkSampleCountFlagBits ConvertSampleCount(uint32_t count);
    static VkFormat              ConvertVertexFormat(VertexFormat format);
    static VkPrimitiveTopology   ConvertTopology(PrimitiveTopology topology);
    static VkPolygonMode         ConvertPolygonMode(PolygonMode mode);
    static VkCullModeFlags       ConvertCullMode(CullMode mode);

    VkDevice              device              = VK_NULL_HANDLE;
    VkPipeline            graphicsPipeline    = VK_NULL_HANDLE;
    VkPipelineLayout      pipelineLayout      = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
};