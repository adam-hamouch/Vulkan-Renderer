#pragma once

class ISampler
{
public:
    virtual ~ISampler() = default;
    
    virtual void Create(IDevice* device, SamplerDesc desc) = 0;
    virtual void Destroy() = 0;
};
