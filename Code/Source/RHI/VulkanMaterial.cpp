#include "../Include/RHI/VulkanMaterial.h"
#include "../Include/RHI/VulkanDevice.h"
#include "../Include/RHI/VulkanBuffer.h"
#include "../Include/RHI/VulkanTexture.h"
#include "../Include/RHI/VulkanSampler.h"
#include "../Include/RHI/VulkanPipeline.h"
#include "../Include/RHI/VulkanCommandList.h"

void VulkanMaterial::Create(IDevice* rhiDevice, MaterialDesc desc)
{
    materialDesc = desc;
    device = static_cast<VulkanDevice*>(rhiDevice)->GetDevice();

    VulkanPipeline* vkPipeline = static_cast<VulkanPipeline*>(desc.pipeline);
    VulkanTexture*  vkTexture  = static_cast<VulkanTexture*>(desc.albedo.get());
    VulkanSampler*  vkSampler  = static_cast<VulkanSampler*>(desc.sampler);

    uint32_t setCount = static_cast<uint32_t>(desc.uniformBuffers.size());

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = setCount;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = setCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = setCount;

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation de la pool de descripteurs!");
    }

    std::vector<VkDescriptorSetLayout> layouts(setCount, vkPipeline->GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = setCount;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(setCount);
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation d'un set de descripteurs!");
    }

    for (size_t i = 0; i < setCount; i++) {
        VulkanBuffer* uniformBuffer = static_cast<VulkanBuffer*>(desc.uniformBuffers[i]);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffer->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = uniformBuffer->GetSize();

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = vkTexture->GetImageView();
        imageInfo.sampler = vkSampler->GetSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void VulkanMaterial::Destroy()
{
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    descriptorPool = VK_NULL_HANDLE;
    descriptorSets.clear();

    materialDesc = {};
}

void VulkanMaterial::Bind(ICommandList* cmdList)
{
    VulkanCommandList* vkCmdList  = static_cast<VulkanCommandList*>(cmdList);
    VulkanPipeline*    vkPipeline = static_cast<VulkanPipeline*>(materialDesc.pipeline);

    vkCmdBindDescriptorSets(vkCmdList->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->GetPipelineLayout(),
        0, 1, &descriptorSets[vkCmdList->GetCurrentFrame()], 0, nullptr);
}
