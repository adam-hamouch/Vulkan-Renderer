#pragma once

enum class ShaderStage
{
    COMPUTE,
    VERTEX,
    FRAGMENT
};

struct ShaderDesc
{
    const char* fileName;
    ShaderStage stage;
};

class IShader
{
public:
    virtual ~IShader() = default;
    
    virtual void Create(ShaderDesc desc, IDevice* device) = 0;
    virtual void Destroy() = 0;
};
