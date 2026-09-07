#pragma once

#include "ISampler.h"

class VulkanSampler : public ISampler
{
public:
    void Create(IDevice* device, SamplerDesc desc) override;
    void Destroy() override;
};
