#pragma once

#include "IQueue.h"
#include "VulkanUtils.h"

class IDevice;

class VulkanQueue : public IQueue
{
public:
    void Create(IDevice* device, QueueType type) override;
    void Destroy() override;

    void Submit(ICommandList* commandList, ISwapChain* swapChain) override;

    VkQueue GetQueue() const { return queue; }

private:
    VkQueue queue = VK_NULL_HANDLE;
};
