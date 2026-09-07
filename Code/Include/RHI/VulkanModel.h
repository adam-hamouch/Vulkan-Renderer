#pragma once

#include "IModel.h"

class VulkanModel : public IModel
{
public:
    void Load(const char* path) override;
    void Destroy() override;
};
