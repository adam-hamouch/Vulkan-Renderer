#pragma once

#include "ISurface.h"

class IInstance;

class VulkanSurface : public ISurface
{
public:
    void Create(IInstance* instance, Window* window) override;
    void Destroy() override;
};
