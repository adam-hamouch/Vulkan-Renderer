#pragma once

class ITexture
{
public:
    virtual ~ITexture() = default;

    virtual void Load(const char* path) = 0;
    virtual void Destroy() = 0;
};
