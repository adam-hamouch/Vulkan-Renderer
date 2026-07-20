#pragma once

class ITexture;
class ICommandList;

struct MaterialDesc
{
    ITexture* albedo = nullptr;;
    ITexture* normal = nullptr;;
};

class IMaterial
{
public:
    virtual ~IMaterial() = default;
    
    virtual void Load(const char* path) = 0;
    virtual void Destroy() = 0;

    virtual void Bind(ICommandList* cmdList);
    
private:
    MaterialDesc materialDesc;    
};
