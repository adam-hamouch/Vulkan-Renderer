#pragma once

#include "ISwapChain.h"

class VulkanSwapChain : public ISwapChain
{
public:
    void Create(IDevice* device, ISurface* surface, void* windowHandle) override;
    void Destroy() override;

    void Resize(uint32_t width, uint32_t height) override;
    void Present(IQueue* queue) override;
    uint32_t AcquireNextImage() override;
};
