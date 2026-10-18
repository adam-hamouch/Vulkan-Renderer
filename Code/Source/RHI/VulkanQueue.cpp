#include "../Include/RHI/VulkanQueue.h"
#include "../Include/RHI/VulkanDevice.h"
#include "../Include/RHI/VulkanSwapChain.h"
#include "../Include/RHI/VulkanCommandList.h"

void VulkanQueue::Create(IDevice* device, QueueType type)
{
    VulkanDevice* vkDevice = static_cast<VulkanDevice*>(device);
    queue = type == QueueType::PRESENT ? vkDevice->GetPresentQueue() : vkDevice->GetGraphicsQueue();
}

void VulkanQueue::Destroy()
{
    queue = VK_NULL_HANDLE;
}

void VulkanQueue::Submit(ICommandList* commandList, ISwapChain* swapChain)
{
    VulkanCommandList* vkCommandList = static_cast<VulkanCommandList*>(commandList);
    VulkanSwapChain*   vkSwapChain   = static_cast<VulkanSwapChain*>(swapChain);

    VkCommandBuffer commandBuffer = vkCommandList->GetCommandBuffer();

    VkSemaphore waitSemaphores[] = {vkSwapChain->GetImageAvailableSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {vkSwapChain->GetRenderFinishedSemaphore()};

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(queue, 1, &submitInfo, vkSwapChain->GetInFlightFence()) != VK_SUCCESS)
    {
        throw std::runtime_error("échec de l'envoi d'un command buffer!");
    }
}
