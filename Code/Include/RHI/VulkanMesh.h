#pragma once

#include "IMesh.h"

class VulkanMesh : public IMesh
{
public:
    void Create(IDevice* device, IModel* model, IMaterial* material) override;
    void Destroy() override;
};
