#pragma once
#include "RHI/IDevice.h"

class ISwapChain;

class ITexture
{
public:
    virtual ~ITexture() = default;

    virtual void Load(const char* path, IDevice* device, ISwapChain* swapChain) = 0;
    virtual void Destroy() = 0;
};
