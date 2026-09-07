#pragma once

#include "RHI.h"

class VkRHI : public IRHI
{
public:
    IInstance*     CreateInstance()     override;
    IDevice*       CreateDevice()       override;
    ISurface*      CreateSurface()      override;
    ISwapChain*    CreateSwapChain()    override;
    IQueue*        CreateQueue()        override;
    IRenderPass*   CreateRenderPass()   override;
    IPipeline*     CreatePipeline()     override;
    ICommandList*  CreateCommandList()  override;
    IBuffer*       CreateBuffer()       override;
    ITexture*      CreateTexture()      override;
    ISampler*      CreateSampler()      override;
    IShader*       CreateShader()       override;
    IMesh*         CreateMesh()         override;
    IModel*        CreateModel()        override;
    IMaterial*     CreateMaterial()     override;

    void Destroy(IInstance*    instance)    override;
    void Destroy(IDevice*      device)      override;
    void Destroy(ISurface*     surface)     override;
    void Destroy(ISwapChain*   swapChain)   override;
    void Destroy(IQueue*       queue)       override;
    void Destroy(IRenderPass*  renderPass)  override;
    void Destroy(IPipeline*    pipeline)    override;
    void Destroy(ICommandList* commandList) override;
    void Destroy(IBuffer*      buffer)      override;
    void Destroy(ITexture*     texture)     override;
    void Destroy(ISampler*     sampler)     override;
    void Destroy(IShader*      shader)      override;
    void Destroy(IMesh*        mesh)        override;
    void Destroy(IModel*       model)       override;
    void Destroy(IMaterial*    material)    override;
};
