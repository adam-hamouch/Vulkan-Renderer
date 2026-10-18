#include "../Include/RHI/VulkanCommandList.h"
#include "../Include/RHI/VulkanBuffer.h"
#include "../Include/RHI/VulkanPipeline.h"

void VulkanCommandList::Create(IDevice* device)
{
    vkDevice = static_cast<VulkanDevice*>(device);

    QueueFamilyIndices queueFamilyIndices = vkDevice->FindQueueFamilies(vkDevice->GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        
    if (vkCreateCommandPool(vkDevice->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) 
    {
        throw std::runtime_error("échec de la création d'une command pool!");
    }
    
    // Un command buffer par frame en vol
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vkDevice->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("échec de l'allocation de command buffers!");
    }
}

void VulkanCommandList::Destroy()
{
    // Libere aussi les command buffers alloues dans la pool
    vkDestroyCommandPool(vkDevice->GetDevice(), commandPool, nullptr);
    commandPool = VK_NULL_HANDLE;
    commandBuffers.clear();
}

void VulkanCommandList::Begin(uint32_t frameIndex)
{
    currentFrame = frameIndex;
    Reset();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void VulkanCommandList::End()
{
    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void VulkanCommandList::Reset()
{
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
}

void VulkanCommandList::SetViewport(float x, float y, float width, float height)
{
    VkViewport viewport{};
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
}

void VulkanCommandList::SetScicors(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    VkRect2D scissor{};
    scissor.offset = {(int32_t) x, (int32_t) y};
    scissor.extent = {width, height};
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);            
}

void VulkanCommandList::BindVertexBuffer(IBuffer* buffer)
{
    VkBuffer vertexBuffers[] = {static_cast<VulkanBuffer*>(buffer)->GetBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
}

void VulkanCommandList::BindIndexBuffer(IBuffer* buffer)
{
    vkCmdBindIndexBuffer(commandBuffers[currentFrame], static_cast<VulkanBuffer*>(buffer)->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void VulkanCommandList::BindPipeline(IPipeline* pipeline)
{
    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<VulkanPipeline*>(pipeline)->GetPipeline());
}

void VulkanCommandList::DrawIndexed(uint32_t indexCount)
{
    vkCmdDrawIndexed(commandBuffers[currentFrame], indexCount, 1, 0, 0, 0);
}
