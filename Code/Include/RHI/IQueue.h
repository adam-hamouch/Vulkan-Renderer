#pragma once

enum class QueueType
{
    GRAPHICS,
    PRESENT,
    TRANSFER,
    COMPUTE
};

class IQueue
{
public:
    virtual ~IQueue() = default;
    
    virtual void Create(IDevice* device, QueueType type) = 0;
    virtual void Destroy() = 0;
};
