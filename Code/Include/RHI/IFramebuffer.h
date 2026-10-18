#pragma once

class IDevice;
class ISwapChain;
class IRenderPass;

class IFramebuffer
{
public:
    virtual ~IFramebuffer() = default;

    virtual void Create(IDevice* device, ISwapChain* swapChain, IRenderPass* renderPass) = 0;
    virtual void Destroy() = 0;

    // A appeler apres la recreation de la swap chain (resize de la fenetre)
    virtual void Recreate(ISwapChain* swapChain) = 0;
};
