#pragma once

enum class FilterMode
{
    Nearest,
    Linear
};

enum class AddressMode
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

struct SamplerDesc 
{
    FilterMode  magFilter     = FilterMode::Linear;
    FilterMode  minFilter     = FilterMode::Linear;
    AddressMode addressMode   = AddressMode::Repeat;
    float       maxAnisotropy = 16.0f;
};

class IDevice;

class ISampler
{
public:
    virtual ~ISampler() = default;
    
    virtual void Create(IDevice* device, SamplerDesc desc) = 0;
    virtual void Destroy() = 0;
};
