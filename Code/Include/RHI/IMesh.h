#pragma once
#include <memory>

#include "Resources/ResourcesManager.h"

class IDevice;
class IMaterial;
class IModel;
class IBuffer;
class ICommandList;

class IMesh
{
public:
    virtual ~IMesh() = default;

    virtual void Create(IDevice* device, const char* filePath, ResourcesManager* resourcesManager) = 0;
    virtual void Draw(IDevice* device, IMaterial* material, ICommandList* commandList) = 0;
    virtual void Destroy() = 0;

protected:
    std::shared_ptr<IModel> model;
};
