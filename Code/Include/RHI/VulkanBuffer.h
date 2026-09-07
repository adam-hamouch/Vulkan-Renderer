#pragma once

#include "IBuffer.h"

class VulkanBuffer : public IBuffer
{
public:
    void Create(IDevice* device, BufferDesc desc) override;
    void Destroy() override;

    void Upload(const void* data, uint32_t size) override;

    uint64_t GetSize() override;
};
