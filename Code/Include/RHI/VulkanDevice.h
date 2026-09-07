#pragma once

#include "IDevice.h"

class VulkanDevice : public IDevice
{
public:
    void Create() override;
    void Destroy() override;
};
