#pragma once

class IDevice;
class ICommandList;
class ISwapChain;

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

    virtual void Submit(ICommandList* commandList, ISwapChain* swapChain) = 0;
};
