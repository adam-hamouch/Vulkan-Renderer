#pragma once

#include "IInstance.h"
#include "VulkanUtils.h"

class VulkanInstance : public IInstance
{
public:
    void Create(const char* appName) override;
    void Destroy() override;
    
private:
    bool checkValidationLayerSupport();
    VkInstance instance;
};
