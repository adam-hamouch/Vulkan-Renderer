#pragma once
#include <vector>

class ITexture;
class ISampler;
class IPipeline;
class ICommandList;
class IBuffer;
class IDevice;

struct MaterialDesc
{
    ITexture*  albedo    = nullptr;
    ITexture*  normal    = nullptr;
    ISampler*  sampler   = nullptr;
    IPipeline* pipeline  = nullptr;

    // Un uniform buffer par frame en vol
    std::vector<IBuffer*> uniformBuffers;
};

class IMaterial
{
public:
    virtual ~IMaterial() = default;

    virtual void Create(IDevice* device, MaterialDesc desc) = 0;
    virtual void Destroy() = 0;
    virtual void Bind(ICommandList* cmdList) = 0;

protected:
    MaterialDesc materialDesc;
};
