#include "../Include/RHI/VulkanSurface.h"
#include "../Include/RHI/VulkanInstance.h"
#include "../Include/RHI/VulkanUtils.h"
#include "Core/Window/Window.h"

void VulkanSurface::Create(IInstance* instance, Window* window)
{
    VulkanInstance* vkInstance = static_cast<VulkanInstance*>(instance);
    this->instance = vkInstance->GetVkInstance();

    if (glfwCreateWindowSurface(this->instance, window->GetHandle(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création de la window surface!");
    }
}

void VulkanSurface::Destroy()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
