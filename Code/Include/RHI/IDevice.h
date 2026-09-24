#pragma once

class IInstance;
class ISurface;

class IDevice
{
public:
    virtual ~IDevice() = default;
    
    virtual void Create(IInstance* instance, ISurface* surface) = 0;
    virtual void Destroy() = 0;
};
