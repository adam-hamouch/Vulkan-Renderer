#include "../Include/RHI/VulkanCommandList.h"
void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) 
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;
        
        std::array<VkClearValue, 3> clearValues{};
        clearValues[0].color        = {{0.0f, 0.0f, 0.0f, 1.0f}}; 
        clearValues[1].depthStencil = {1.0f, 0};                  
        clearValues[2].color        = {{0.0f, 0.0f, 0.0f, 1.0f}}; 

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);            
        
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
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
