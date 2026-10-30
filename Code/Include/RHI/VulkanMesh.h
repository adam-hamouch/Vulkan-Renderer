#pragma once

#include "IMesh.h"
#include "IModel.h"

class VulkanMesh : public IMesh
{
public:
    void Create(IDevice* device, const char* filePath, ResourcesManager* resourcesManager) override;
    void Draw(IDevice* device, IMaterial* material, ICommandList* commandList) override;
    void Destroy() override;
    
private :
    IBuffer* vertexBuffer = nullptr;
    IBuffer* indexBuffer  = nullptr;
};
