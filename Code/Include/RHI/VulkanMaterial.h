#pragma once

#include "IMaterial.h"

class VulkanMaterial : public IMaterial
{
public:
    void Load(const char* path) override;
    void Destroy() override;

    void Bind(ICommandList* cmdList) override;
};
