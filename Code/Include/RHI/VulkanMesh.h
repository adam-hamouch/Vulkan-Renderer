#pragma once

#include "IMesh.h"

class VulkanMesh : public IMesh
{
public:
    void Create(IDevice* device, std::shared_ptr<IModel> model) override;
    void Destroy() override;
};
