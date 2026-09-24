#pragma once

#include "IMaterial.h"

class VulkanMaterial : public IMaterial
{
public:
    void Create(MaterialDesc desc) override;
    void Destroy() override;

    void Bind(ICommandList* cmdList) override;
};
