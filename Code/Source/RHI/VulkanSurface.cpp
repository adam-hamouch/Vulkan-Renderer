#include "../Include/RHI/VulkanSurface.h"
#include "../Include/RHI/VulkanUtils.h"

void VulkanSurface::Create(IInstance* instance, Window* window)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création de la window surface!");
    }
}

void VulkanSurface::Destroy()
{
}
