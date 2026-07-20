#pragma once

class IDevice;
class IMaterial;
class IModel;
class IBuffer;

class IMesh
{
public:
    virtual ~IMesh() = default;
    
    virtual void Create(IDevice* device, IModel* model, IMaterial* material) = 0;
    virtual void Destroy() = 0;
    
private:
    IBuffer* vertexBuffer;
    IBuffer* indexBuffer;

    IMaterial* material;
};
