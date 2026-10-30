#pragma once
#include <memory>

class IDevice;
class IMaterial;
class IModel;
class IBuffer;
class ICommandList;
class ResourcesManager;

class IMesh
{
public:
    virtual ~IMesh() = default;

    virtual void Create(IDevice* device, const char* filePath, ResourcesManager* resourcesManager) = 0;
    virtual void Draw(IMaterial* material, ICommandList* commandList) = 0;
    virtual void Destroy() = 0;

protected:
    std::shared_ptr<IModel> model;
};
