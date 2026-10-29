#pragma once
#include <memory>

class IDevice;
class IMaterial;
class IModel;
class IBuffer;

class IMesh
{
public:
    virtual ~IMesh() = default;

    virtual void Create(IDevice* device, std::shared_ptr<IModel> model) = 0;
    virtual void Destroy() = 0;

protected:
    // Possede par le ResourcesManager : le mesh le garde seulement en vie tant qu'il existe
    std::shared_ptr<IModel> model;

    IBuffer* vertexBuffer = nullptr;
    IBuffer* indexBuffer  = nullptr;

    IMaterial* material = nullptr;
};
