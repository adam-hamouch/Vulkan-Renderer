#include "LowRenderer/Renderer.h"

#include "Core/Scene.h"
#include "Core/Window/Window.h"
#include "RHI/VkRHI.h"
#include "RHI/VulkanUtils.h"
#include "LowRenderer/UniformBufferObjectt.h"
#include "Resources/ResourcesManager.h"

void Renderer::Init(Window& window)
{
    rhi = new VkRHI();

    instance    = rhi->CreateInstance();
    surface     = rhi->CreateSurface();
    device      = rhi->CreateDevice();
    swapChain   = rhi->CreateSwapChain();
    queue       = rhi->CreateQueue();
    renderPass  = rhi->CreateRenderPass();
    framebuffer = rhi->CreateFramebuffer();
    pipeline    = rhi->CreatePipeline();
    commandList = rhi->CreateCommandList();
    sampler     = rhi->CreateSampler();

    instance->Create("VulkanRenderer");
    surface->Create(instance, &window);
    device->Create(instance, surface);

    resourcesManager = new ResourcesManager(rhi, device);

    swapChain->Create(device, surface, &window);
    queue->Create(device, QueueType::GRAPHICS);

    RenderPassDesc renderPassDesc;
    renderPassDesc.attachments = {
        {AttachmentFormat::BGRA8,   LoadOp::CLEAR,    StoreOp::STORE,    false, false, 4},
        {AttachmentFormat::Depth32, LoadOp::CLEAR,    StoreOp::DONTCARE, true,  false, 4},
        {AttachmentFormat::BGRA8,   LoadOp::DONTCARE, StoreOp::STORE,    false, true,  1}
    };
    renderPassDesc.subPasses = { {{0}, {}, 1} };
    renderPass->Create(device, renderPassDesc);
    framebuffer->Create(device, swapChain, renderPass);

    vertShader = resourcesManager->Load<IShader>("../../Ressources/Shaders/vert.spv", ShaderStage::VERTEX);
    fragShader = resourcesManager->Load<IShader>("../../Ressources/Shaders/frag.spv", ShaderStage::FRAGMENT);

    PipelineDesc pipelineDesc;
    pipelineDesc.shaders = {vertShader.get(), fragShader.get()};
    pipelineDesc.layout.stride     = sizeof(Vertex);
    pipelineDesc.layout.attributes = {
        {0, VertexFormat::Float3, offsetof(Vertex, pos)},
        {1, VertexFormat::Float3, offsetof(Vertex, color)},
        {2, VertexFormat::Float2, offsetof(Vertex, texCoord)}
    };
    pipelineDesc.topology    = PrimitiveTopology::TriangleList;
    pipelineDesc.polygonMode = PolygonMode::Fill;
    pipelineDesc.cullMode    = CullMode::Back;
    pipelineDesc.depthTest   = true;
    pipelineDesc.depthWrite  = true;
    pipelineDesc.blendEnable = false;
    pipelineDesc.sampleCount = 4;
    pipelineDesc.renderPass  = renderPass;
    pipeline->Create(device, pipelineDesc);

    commandList->Create(device);

    SamplerDesc samplerDesc;
    sampler->Create(device, samplerDesc);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (IBuffer*& uniformBuffer : uniformBuffers)
    {
        uniformBuffer = rhi->CreateBuffer();
        uniformBuffer->Create(device, { MemoryType::CPU_To_GPU, BufferUsage::Uniform, sizeof(UniformBufferObject) });
    }
}

void Renderer::Draw(Window& window, Scene& scene)
{
    uint32_t imageIndex = swapChain->AcquireNextImage();
    uint32_t frame      = swapChain->GetCurrentFrame();

    UniformBufferObject ubo{};
    ubo.model = GpuMat4(scene.modelMatrix.Transpose());
    ubo.view  = GpuMat4(scene.camera.GetViewMatrix().Transpose());
    Mat4 proj = scene.camera.GetProjectionMatrix();
    proj.data[5] *= -1.0f;
    ubo.proj  = GpuMat4(proj.Transpose());
    uniformBuffers[frame]->Upload(&ubo, sizeof(ubo));

    commandList->Begin(frame);
    renderPass->Begin(commandList, framebuffer, imageIndex);

    commandList->SetViewport(0.0f, 0.0f, (float) window.GetWidth(), (float) window.GetHeight());
    commandList->SetScicors(0, 0, window.GetWidth(), window.GetHeight());

    commandList->BindPipeline(pipeline);
    scene.mesh->Draw(scene.material, commandList);

    renderPass->End(commandList);
    commandList->End();

    queue->Submit(commandList, swapChain);
    swapChain->Present(queue);
}

void Renderer::Resize(Window& window)
{
    window.WaitWhileMinimized();
    device->WaitIdle();

    framebuffer->Destroy();
    swapChain->Resize(window.GetWidth(), window.GetHeight());
    framebuffer->Create(device, swapChain, renderPass);
}

void Renderer::WaitIdle()
{
    device->WaitIdle();
}

void Renderer::Destroy()
{
    for (IBuffer* uniformBuffer : uniformBuffers)
        rhi->Destroy(uniformBuffer);
    uniformBuffers.clear();

    rhi->Destroy(sampler);
    rhi->Destroy(commandList);
    rhi->Destroy(pipeline);

    vertShader.reset();
    fragShader.reset();
    delete resourcesManager;

    rhi->Destroy(framebuffer);
    rhi->Destroy(renderPass);
    rhi->Destroy(swapChain);
    rhi->Destroy(queue);

    rhi->Destroy(device);
    rhi->Destroy(surface);
    rhi->Destroy(instance);

    delete rhi;
}
