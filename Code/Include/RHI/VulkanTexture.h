#pragma once

#include "ITexture.h"

class VulkanTexture : public ITexture
{
public:
    void Load(const char* path) override;
    void Destroy() override;
};
