#include "../Include/RHI/VulkanCommandList.h"

void VulkanCommandList::Create(IDevice* device)
{
}

void VulkanCommandList::Destroy()
{
}

void VulkanCommandList::Begin()
{
}

void VulkanCommandList::End()
{
}

void VulkanCommandList::Reset()
{
}

void VulkanCommandList::SetViewport(float x, float y, float width, float height)
{
}

void VulkanCommandList::SetScicors(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
}

void VulkanCommandList::BindVertexBuffer(IBuffer* buffer)
{
}

void VulkanCommandList::BindIndexBuffer(IBuffer* buffer)
{
}

void VulkanCommandList::BindBuffer(uint32_t binding, IBuffer* buffer)
{
}

void VulkanCommandList::BindTexture(uint32_t binding, ITexture* texture, ISampler* sampler)
{
}

void VulkanCommandList::BindPipeline(IPipeline* pipeline)
{
}

void VulkanCommandList::DrawIndexed(uint32_t indexCount)
{
}
