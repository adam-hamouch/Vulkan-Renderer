#pragma once

class IInstance
{
public:
    virtual ~IInstance() = default;
    
    virtual void Create(const char * appName) = 0;
    virtual void Destroy() = 0;
};
