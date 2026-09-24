#pragma once

class ITexture;
class ISampler;
class IPipeline;
class ICommandList;

struct MaterialDesc
{
    ITexture*  albedo    = nullptr;
    ITexture*  normal    = nullptr;
    ISampler*  sampler   = nullptr;
    IPipeline* pipeline  = nullptr;
};

class IMaterial
{
public:
    virtual ~IMaterial() = default;

    virtual void Create(MaterialDesc desc) = 0;
    virtual void Destroy() = 0;
    virtual void Bind(ICommandList* cmdList) = 0;

protected:
    MaterialDesc materialDesc;
};
