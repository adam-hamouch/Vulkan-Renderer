#pragma once
#include <cstdint>

class IDevice;
class ISurface;
class IQueue;

class ISwapChain
{
public:
    virtual ~ISwapChain() = default;

    virtual void Create(IDevice* device, ISurface* surface, void* windowHandle) = 0;
    virtual void Destroy() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual void Present(IQueue* queue) = 0;
    virtual uint32_t AcquireNextImage() = 0;
};