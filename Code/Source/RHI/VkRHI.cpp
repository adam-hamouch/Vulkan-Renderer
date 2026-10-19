#include "RHI/VkRHI.h"
#include "RHI/RHIUtils.h"
#include "RHI/IMesh.h"
#include "RHI/VulkanInstance.h"
#include "RHI/VulkanSurface.h"
#include "RHI/VulkanDevice.h"
#include "RHI/VulkanSwapChain.h"
#include "RHI/VulkanQueue.h"
#include "RHI/VulkanRenderPass.h"
#include "RHI/VulkanFramebuffer.h"
#include "RHI/VulkanPipeline.h"
#include "RHI/VulkanCommandList.h"
#include "RHI/VulkanBuffer.h"
#include "RHI/VulkanTexture.h"
#include "RHI/VulkanSampler.h"
#include "RHI/VulkanShader.h"
#include "RHI/VulkanMesh.h"
#include "RHI/VulkanModel.h"
#include "RHI/VulkanMaterial.h"
#include "RHI/VulkanImage.h"

IInstance* VkRHI::CreateInstance()
{
    return new VulkanInstance();
}

IDevice* VkRHI::CreateDevice()
{
    return new VulkanDevice();
}

ISurface* VkRHI::CreateSurface()
{
    return new VulkanSurface();
}

ISwapChain* VkRHI::CreateSwapChain()
{
    return new VulkanSwapChain();
}

IQueue* VkRHI::CreateQueue()
{
    return new VulkanQueue();
}

IRenderPass* VkRHI::CreateRenderPass()
{
    return new VulkanRenderPass();
}

IFramebuffer* VkRHI::CreateFramebuffer()
{
    return new VulkanFramebuffer();
}

IPipeline* VkRHI::CreatePipeline()
{
    return new VulkanPipeline();
}

ICommandList* VkRHI::CreateCommandList()
{
    return new VulkanCommandList();
}

IBuffer* VkRHI::CreateBuffer()
{
    return new VulkanBuffer();
}

ITexture* VkRHI::CreateTexture()
{
    return new VulkanTexture();
}

ISampler* VkRHI::CreateSampler()
{
    return new VulkanSampler();
}

IShader* VkRHI::CreateShader()
{
    return new VulkanShader();
}

IMesh* VkRHI::CreateMesh()
{
    return new VulkanMesh();
}

IModel* VkRHI::CreateModel()
{
    return new VulkanModel();
}

IMaterial* VkRHI::CreateMaterial()
{
    return new VulkanMaterial();
}

IImage* VkRHI::CreateImage()
{
    return new VulkanImage();
}

void VkRHI::Destroy(IInstance* instance)
{
    instance->Destroy();
    delete instance;
}

void VkRHI::Destroy(IDevice* device)
{
    device->Destroy();
    delete device;
}

void VkRHI::Destroy(ISurface* surface)
{
    surface->Destroy();
    delete surface;
}

void VkRHI::Destroy(ISwapChain* swapChain)
{
    swapChain->Destroy();
    delete swapChain;
}

void VkRHI::Destroy(IQueue* queue)
{
    queue->Destroy();
    delete queue;
}

void VkRHI::Destroy(IRenderPass* renderPass)
{
    renderPass->Destroy();
    delete renderPass;
}

void VkRHI::Destroy(IFramebuffer* framebuffer)
{
    framebuffer->Destroy();
    delete framebuffer;
}

void VkRHI::Destroy(IPipeline* pipeline)
{
    pipeline->Destroy();
    delete pipeline;
}

void VkRHI::Destroy(ICommandList* commandList)
{
    commandList->Destroy();
    delete commandList;
}

void VkRHI::Destroy(IBuffer* buffer)
{
    buffer->Destroy();
    delete buffer;
}

void VkRHI::Destroy(ITexture* texture)
{
    texture->Destroy();
    delete texture;
}

void VkRHI::Destroy(ISampler* sampler)
{
    sampler->Destroy();
    delete sampler;
}

void VkRHI::Destroy(IShader* shader)
{
    shader->Destroy();
    delete shader;
}

void VkRHI::Destroy(IMesh* mesh)
{
    mesh->Destroy();
    delete mesh;
}

void VkRHI::Destroy(IModel* model)
{
    model->Destroy();
    delete model;
}

void VkRHI::Destroy(IMaterial* material)
{
    material->Destroy();
    delete material;
}

void VkRHI::Destroy(IImage* image)
{
    image->Destroy();
    delete image;
}
