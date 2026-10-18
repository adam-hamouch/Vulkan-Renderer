#pragma once

#include "IBuffer.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"

class VulkanBuffer : public IBuffer
{
public:
    void Create(IDevice* device, BufferDesc desc) override;
    void Copy(IBuffer* src, IBuffer* dst, uint64_t size) override;
    void Destroy() override;

    void Upload(const void* data, uint64_t size) override;

    uint64_t GetSize() override;

    VkBuffer       GetBuffer() const { return buffer; }
    VkDeviceMemory GetMemory() const { return bufferMemory; }

private :
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkBuffer       buffer       = VK_NULL_HANDLE;
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
    BufferDesc     desc{};
    VulkanDevice*  vkDevice     = nullptr;
};
