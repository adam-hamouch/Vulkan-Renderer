#pragma once

#include "ICommandList.h"

class VulkanCommandList : public ICommandList
{
public:
    void Create(IDevice* device) override;
    void Destroy() override;

    void Begin() override;
    void End() override;
    void Reset() override;

    void SetViewport(float x, float y, float width, float height) override;
    void SetScicors(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void BindVertexBuffer(IBuffer* buffer) override;
    void BindIndexBuffer(IBuffer* buffer) override;

    void BindBuffer(uint32_t binding, IBuffer* buffer) override;
    void BindTexture(uint32_t binding, ITexture* texture, ISampler* sampler) override;

    void BindPipeline(IPipeline* pipeline) override;
    void DrawIndexed(uint32_t indexCount) override;
};
