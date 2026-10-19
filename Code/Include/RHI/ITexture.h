#pragma once
#include "RHI/IDevice.h"

class ITexture
{
public:
    virtual ~ITexture() = default;

    virtual void Load(const char* path, IDevice* device) = 0;
    virtual void Destroy() = 0;
};
