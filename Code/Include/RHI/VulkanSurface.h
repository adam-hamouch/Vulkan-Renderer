#pragma once

#include "ISurface.h"
#include "../Include/RHI/VulkanUtils.h"

class IInstance;

class VulkanSurface : public ISurface
{
public:
    void Create(IInstance* instance, Window* window) override;
    void Destroy() override;
    
    VkSurfaceKHR GetVkSurfaceKHR() { return surface; } 
    
private :
    VkSurfaceKHR surface;
};
