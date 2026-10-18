#include "../Include/RHI/VulkanBuffer.h"

#include <cstring>

void VulkanBuffer::Create(IDevice* device, BufferDesc desc)
{
    vkDevice = static_cast<VulkanDevice*>(device);
    this->desc = desc;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = desc.size;

    switch (desc.usage) {
    case BufferUsage::Vertex  : bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;  break;
    case BufferUsage::Uniform : bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT; break;
    case BufferUsage::Index   : bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;   break;
    case BufferUsage::Staging : bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;   break;
    case BufferUsage::Storage : bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT; break;
    }

    VkMemoryPropertyFlags properties = 0;
    switch (desc.type) {
    case MemoryType::CPU_To_GPU :
        properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        break;
    case MemoryType::GPU_Only :
        properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        break;
    }

    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkDevice device_ = vkDevice->GetDevice();

    if (vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'un buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de memoire!");
    }

    vkBindBufferMemory(device_, buffer, bufferMemory, 0);
}

void VulkanBuffer::Copy(IBuffer* src, IBuffer* dst, uint64_t size)
{
    VkBuffer srcBuffer = static_cast<VulkanBuffer*>(src)->GetBuffer();
    VkBuffer dstBuffer = static_cast<VulkanBuffer*>(dst)->GetBuffer();

    VkCommandBuffer commandBuffer = vkDevice->BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkDevice->EndSingleTimeCommands(commandBuffer);
}

void VulkanBuffer::Destroy()
{
    if (!vkDevice)
        return;

    vkDestroyBuffer(vkDevice->GetDevice(), buffer, nullptr);
    vkFreeMemory(vkDevice->GetDevice(), bufferMemory, nullptr);

    buffer       = VK_NULL_HANDLE;
    bufferMemory = VK_NULL_HANDLE;
}

void VulkanBuffer::Upload(const void* data, uint64_t size)
{
    if (size > desc.size) {
        throw std::runtime_error("upload plus grand que le buffer!");
    }

    if (desc.type == MemoryType::CPU_To_GPU) {
        void* mapped;
        vkMapMemory(vkDevice->GetDevice(), bufferMemory, 0, size, 0, &mapped);
        memcpy(mapped, data, (size_t) size);
        vkUnmapMemory(vkDevice->GetDevice(), bufferMemory);
        return;
    }

    VulkanBuffer stagingBuffer;
    stagingBuffer.Create(vkDevice, { MemoryType::CPU_To_GPU, BufferUsage::Staging, size });
    stagingBuffer.Upload(data, size);

    Copy(&stagingBuffer, this, size);

    stagingBuffer.Destroy();
}

uint64_t VulkanBuffer::GetSize()
{
    return desc.size;
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkDevice->GetPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("aucun type de memoire ne satisfait le buffer!");
}
