#pragma once
#include <cstdint>

class IDevice;

enum class MemoryType
{
    CPU_To_GPU,
    GPU_Only
};

enum class BufferUsage
{
    Vertex,
    Uniform,
    Index,
    Staging,   
    Storage
};

struct BufferDesc
{
    MemoryType  type;
    BufferUsage usage;
    uint64_t    size;
};

class IBuffer
{
public:
    virtual ~IBuffer() = default;
    
    virtual void Create(IDevice* device, BufferDesc desc) = 0;
    virtual void Destroy() = 0;

    virtual void Upload(const void* data, uint32_t size) = 0;

    virtual uint64_t GetSize() = 0;
    
};
