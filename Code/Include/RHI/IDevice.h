#pragma once

class IInstance;
class ISurface;

class IDevice
{
public:
    virtual ~IDevice() = default;
    
    virtual void Create(IInstance* instance, ISurface* surface) = 0;
    virtual void Destroy() = 0;

    // Attend que le GPU ait fini tout le travail en cours (avant destruction / resize)
    virtual void WaitIdle() = 0;
};
