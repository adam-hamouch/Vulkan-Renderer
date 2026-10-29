#include "../Include/RHI/VulkanMesh.h"

void VulkanMesh::Create(IDevice* device, std::shared_ptr<IModel> model)
{
    this->model = std::move(model);
}

void VulkanMesh::Destroy()
{
    // Rend la reference sur le model, le manager reste seul juge de sa destruction
    model.reset();
}
