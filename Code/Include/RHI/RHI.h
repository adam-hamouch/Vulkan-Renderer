#pragma once

class IBuffer;
class ICommandList;
class IInstance;
class IMaterial;
class IModel;
class IPipeline;
class IQueue;
class IRenderPass;
class ISampler;
class IShader;
class ISurface;
class ISwapChain;
class ITexture;
class IDevice;
class IMesh;

class IRHI
{
public:
    virtual ~IRHI() = default;

    virtual IInstance*     CreateInstance()     = 0;
    virtual IDevice*       CreateDevice()       = 0;
    virtual ISurface*      CreateSurface()      = 0;
    virtual ISwapChain*    CreateSwapChain()    = 0;
    virtual IQueue*        CreateQueue()        = 0;
    virtual IRenderPass*   CreateRenderPass()   = 0;
    virtual IPipeline*     CreatePipeline()     = 0;
    virtual ICommandList*  CreateCommandList()  = 0;
    virtual IBuffer*       CreateBuffer()       = 0;
    virtual ITexture*      CreateTexture()      = 0;
    virtual ISampler*      CreateSampler()      = 0;
    virtual IShader*       CreateShader()       = 0;
    virtual IMesh*         CreateMesh()         = 0;
    virtual IModel*        CreateModel()        = 0;
    virtual IMaterial*     CreateMaterial()     = 0;

    virtual void Destroy(IInstance*    instance)    = 0;
    virtual void Destroy(IDevice*      device)      = 0;
    virtual void Destroy(ISurface*     surface)     = 0;
    virtual void Destroy(ISwapChain*   swapChain)   = 0;
    virtual void Destroy(IQueue*       queue)       = 0;
    virtual void Destroy(IRenderPass*  renderPass)  = 0;
    virtual void Destroy(IPipeline*    pipeline)    = 0;
    virtual void Destroy(ICommandList* commandList) = 0;
    virtual void Destroy(IBuffer*      buffer)      = 0;
    virtual void Destroy(ITexture*     texture)     = 0;
    virtual void Destroy(ISampler*     sampler)     = 0;
    virtual void Destroy(IShader*      shader)      = 0;
    virtual void Destroy(IMesh*        mesh)        = 0;
    virtual void Destroy(IModel*       model)       = 0;
    virtual void Destroy(IMaterial*    material)    = 0;
};
