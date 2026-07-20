#pragma once

class IDevice
{
public:
    virtual ~IDevice() = default;
    
    virtual void Create() = 0;
    virtual void Destroy() = 0;
};
