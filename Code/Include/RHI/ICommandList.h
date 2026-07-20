#pragma once

class IDevice;
class IBuffer;
class ISampler;
class ITexture;
class IPipeline;

class ICommandList
{
public:
    virtual ~ICommandList() = default;
    
    virtual void Create(IDevice* device) = 0;
    virtual void Destroy() = 0;

    virtual void Begin() = 0;
    virtual void End() = 0;
    virtual void Reset() = 0;

    virtual void SetViewport(float x, float y, float width, float height) = 0;
    virtual void SetScicors(uint32_t  x, uint32_t  y, uint32_t  width, uint32_t  height) = 0;

    virtual void BindVertexBuffer(IBuffer* buffer) = 0;
    virtual void BindIndexBuffer(IBuffer* buffer) = 0;

    virtual void BindBuffer(uint32_t  binding, IBuffer* buffer) = 0;
    virtual void BindTexture(uint32_t  binding, ITexture* texture, ISampler* sampler) = 0;

    virtual void BindPipeline(IPipeline* pipeline) = 0;
    virtual void DrawIndexed(uint32_t  indexCount) = 0;
};
