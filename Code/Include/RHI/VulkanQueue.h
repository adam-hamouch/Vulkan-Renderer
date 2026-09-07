#pragma once

#include "IQueue.h"

class IDevice;

class VulkanQueue : public IQueue
{
public:
    void Create(IDevice* device, QueueType type) override;
    void Destroy() override;
};
