#pragma once

#include "RHI/RHI.h"
#include "RHI/RHIUtils.h"

#include <vector>

class Window;
class Scene;
class ResourcesManager;

class Renderer
{
public:
    void Init(Window& window);
    void Draw(Window& window, Scene& scene);
    void Resize(Window& window);
    void WaitIdle();
    void Destroy();

    IRHI*                        GetRHI()              const { return rhi; }
    IDevice*                     GetDevice()           const { return device; }
    ResourcesManager*            GetResourcesManager() const { return resourcesManager; }
    IPipeline*                   GetPipeline()         const { return pipeline; }
    ISampler*                    GetSampler()          const { return sampler; }
    const std::vector<IBuffer*>& GetUniformBuffers()   const { return uniformBuffers; }

private:
    IRHI* rhi = nullptr;

    ResourcesManager* resourcesManager = nullptr;

    IInstance*    instance    = nullptr;
    ISurface*     surface     = nullptr;
    IDevice*      device      = nullptr;
    ISwapChain*   swapChain   = nullptr;
    IQueue*       queue       = nullptr;
    IRenderPass*  renderPass  = nullptr;
    IFramebuffer* framebuffer = nullptr;
    IPipeline*    pipeline    = nullptr;
    ICommandList* commandList = nullptr;
    ISampler*     sampler     = nullptr;

    std::shared_ptr<IShader> vertShader;
    std::shared_ptr<IShader> fragShader;

    std::vector<IBuffer*> uniformBuffers;
};
