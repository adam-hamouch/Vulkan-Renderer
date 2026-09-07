#pragma once

#include "IShader.h"

class IDevice;

class VulkanShader : public IShader
{
public:
    void Create(ShaderDesc desc, IDevice* device) override;
    void Destroy() override;
};
