#pragma once

class Window;
class IInstance;

class ISurface
{
public:
    virtual ~ISurface() = default;
    
    virtual void Create(IInstance* instance, Window* window) = 0;
    virtual void Destroy() = 0;
};
