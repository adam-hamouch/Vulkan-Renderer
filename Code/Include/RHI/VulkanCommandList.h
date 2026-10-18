#pragma once

#include "ICommandList.h"
#include "RHI/VulkanUtils.h"
#include "RHI/VulkanDevice.h"

class VulkanCommandList : public ICommandList
{
public:
    void Create(IDevice* device) override;
    void Destroy() override;

    void Begin(uint32_t frameIndex) override;
    void End() override;
    void Reset() override;

    void SetViewport(float x, float y, float width, float height) override;
    void SetScicors(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void BindVertexBuffer(IBuffer* buffer) override;
    void BindIndexBuffer(IBuffer* buffer) override;

    void BindPipeline(IPipeline* pipeline) override;
    void DrawIndexed(uint32_t indexCount) override;

    VkCommandBuffer GetCommandBuffer() const { return commandBuffers[currentFrame]; }
    uint32_t        GetCurrentFrame()  const { return currentFrame; }

private:
    VulkanDevice* vkDevice = nullptr;

    VkCommandPool                commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;
    uint32_t                     currentFrame = 0;
};
