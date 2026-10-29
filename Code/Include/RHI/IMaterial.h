#pragma once
#include <memory>
#include <vector>

class ITexture;
class ISampler;
class IPipeline;
class ICommandList;
class IBuffer;
class IDevice;

struct MaterialDesc
{
    // Possedees par le ResourcesManager : le material les garde seulement en vie tant qu'il existe
    std::shared_ptr<ITexture> albedo;
    std::shared_ptr<ITexture> normal;

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
