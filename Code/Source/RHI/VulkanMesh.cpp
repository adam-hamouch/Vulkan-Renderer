#include "../Include/RHI/VulkanMesh.h"
#include "Resources/ResourcesManager.h"

#include "RHI/VulkanBuffer.h"
#include "RHI/ICommandList.h"
#include "RHI/IMaterial.h"
#include "RHI/IModel.h"

void VulkanMesh::Create(IDevice* device, const char* filePath, ResourcesManager* resourcesManager)
{
    vertexBuffer = new VulkanBuffer();
    indexBuffer  = new VulkanBuffer();
    
    model = resourcesManager->Load<IModel>(filePath);
    const std::vector<Vertex>&   vertices = model->GetVertices();
    const std::vector<uint32_t>& indices  = model->GetIndices();

    BufferDesc vertexBufferDesc;
    vertexBufferDesc.usage = BufferUsage::Vertex;
    vertexBufferDesc.type  = MemoryType::GPU_Only;
    vertexBufferDesc.size  = sizeof(Vertex) * vertices.size();
    vertexBuffer->Create(device, vertexBufferDesc);
    vertexBuffer->Upload(vertices.data(), sizeof(Vertex) * vertices.size());

    BufferDesc indexBufferDesc;
    indexBufferDesc.usage = BufferUsage::Index;
    indexBufferDesc.type  = MemoryType::GPU_Only;
    indexBufferDesc.size  = sizeof(uint32_t) * indices.size();
    indexBuffer->Create(device, indexBufferDesc);
    indexBuffer->Upload(indices.data(), sizeof(uint32_t) * indices.size());
}

void VulkanMesh::Draw(IMaterial* material, ICommandList* commandList)
{
    commandList->BindVertexBuffer(vertexBuffer);
    commandList->BindIndexBuffer(indexBuffer);
    
    if (material)
        material->Bind(commandList);

    commandList->DrawIndexed(static_cast<uint32_t>(model->GetIndices().size()));
}

void VulkanMesh::Destroy()
{
    if (indexBuffer)
    {
        indexBuffer->Destroy();
        delete indexBuffer;
        indexBuffer = nullptr;
    }
    if (vertexBuffer)
    {
        vertexBuffer->Destroy();
        delete vertexBuffer;
        vertexBuffer = nullptr;
    }
    model.reset();
}
