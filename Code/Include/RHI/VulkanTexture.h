#pragma once

#include "ITexture.h"
#include <cstdint>

class VulkanTexture : public ITexture
{
public:
    void Load(const char* path, IDevice* device) override;
    void Destroy() override;

private :
    uint32_t mipLevels = 1; 
};
