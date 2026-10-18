#pragma once

#include "Core/Window/Window.h"
#include "RHI/RHIUtils.h"
#include "RHI/RHI.h"
#include "RHI/VkRHI.h"
#include "RHI/VulkanUtils.h"
#include "LowRenderer/Camera.h"
#include "LowRenderer/UniformBufferObjectt.h"

#include <chrono>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Application
{
public:
    void run()
    {
        InitWindow();
        InitRHI();
        InitRenderingEngine();
        RenderLoop();
        CleanUp();
    }

    void InitWindow()
    {
        window = new Window();
        window->Create("Vulkan Renderer", WIDTH, HEIGHT);
    }

    void InitRHI()
    {
        rhi = new VkRHI();

        rhiInstance    = rhi->CreateInstance();
        rhiSurface     = rhi->CreateSurface();
        rhiDevice      = rhi->CreateDevice();
        rhiSwapChain   = rhi->CreateSwapChain();
        rhiQueue       = rhi->CreateQueue();
        rhiRenderPass  = rhi->CreateRenderPass();
        rhiFramebuffer = rhi->CreateFramebuffer();
        vertShader     = rhi->CreateShader();
        fragShader     = rhi->CreateShader();
        rhiPipeline    = rhi->CreatePipeline();
        rhiCommandList = rhi->CreateCommandList();
        vertexBuffer   = rhi->CreateBuffer();
        indexBuffer    = rhi->CreateBuffer();
        rhiTexture     = rhi->CreateTexture();
        rhiSampler     = rhi->CreateSampler();
        rhiModel       = rhi->CreateModel();
        rhiMesh        = rhi->CreateMesh();
        rhiMaterial    = rhi->CreateMaterial();

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        for (IBuffer*& uniformBuffer : uniformBuffers)
            uniformBuffer = rhi->CreateBuffer();
    }

    void InitRenderingEngine()
    {
        rhiInstance->Create("VulkanRenderer");
        rhiSurface->Create(rhiInstance, window);
        rhiDevice->Create(rhiInstance, rhiSurface);
        rhiSwapChain->Create(rhiDevice, rhiSurface, window);
        rhiQueue->Create(rhiDevice, QueueType::GRAPHICS);

        InitCamera();

        RenderPassDesc renderPassDesc;
        renderPassDesc.attachments = {
            {AttachmentFormat::BGRA8,   LoadOp::CLEAR,    StoreOp::STORE,    false, false, 4},
            {AttachmentFormat::Depth32, LoadOp::CLEAR,    StoreOp::DONTCARE, true,  false, 4},
            {AttachmentFormat::BGRA8,   LoadOp::DONTCARE, StoreOp::STORE,    false, true,  1}
        };
        renderPassDesc.subPasses = { {{0}, {}, 1} };
        rhiRenderPass->Create(rhiDevice, renderPassDesc);
        rhiFramebuffer->Create(rhiDevice, rhiSwapChain, rhiRenderPass);

        ShaderDesc vertDesc{"../../Ressources/Shaders/vert.spv", ShaderStage::VERTEX};
        ShaderDesc fragDesc{"../../Ressources/Shaders/frag.spv", ShaderStage::FRAGMENT};
        vertShader->Create(vertDesc, rhiDevice);
        fragShader->Create(fragDesc, rhiDevice);

        PipelineDesc pipelineDesc;
        pipelineDesc.shaders     = {vertShader, fragShader};
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
        pipelineDesc.renderPass  = rhiRenderPass;
        rhiPipeline->Create(rhiDevice, pipelineDesc);

        rhiCommandList->Create(rhiDevice);

        rhiTexture->Load("../../Ressources/Textures/viking_room.png", rhiDevice, rhiSwapChain);

        SamplerDesc samplerDesc;
        rhiSampler->Create(rhiDevice, samplerDesc);

        for (IBuffer* uniformBuffer : uniformBuffers)
            uniformBuffer->Create(rhiDevice, { MemoryType::CPU_To_GPU, BufferUsage::Uniform, sizeof(UniformBufferObject) });

        rhiMaterial->Create(rhiDevice, {rhiTexture, nullptr, rhiSampler, rhiPipeline, uniformBuffers});

        rhiModel->Load("../../Ressources/Models/viking_room.obj");
        const std::vector<Vertex>&   vertices = rhiModel->GetVertices();
        const std::vector<uint32_t>& indices  = rhiModel->GetIndices();

        BufferDesc vertexBufferDesc;
        vertexBufferDesc.usage = BufferUsage::Vertex;
        vertexBufferDesc.type  = MemoryType::GPU_Only;
        vertexBufferDesc.size  = sizeof(Vertex) * vertices.size();
        vertexBuffer->Create(rhiDevice, vertexBufferDesc);
        vertexBuffer->Upload(vertices.data(), sizeof(Vertex) * vertices.size());

        BufferDesc indexBufferDesc;
        indexBufferDesc.usage = BufferUsage::Index;
        indexBufferDesc.type  = MemoryType::GPU_Only;
        indexBufferDesc.size  = sizeof(uint32_t) * indices.size();
        indexBuffer->Create(rhiDevice, indexBufferDesc);
        indexBuffer->Upload(indices.data(), sizeof(uint32_t) * indices.size());

        rhiMesh->Create(rhiDevice, rhiModel, rhiMaterial);
    }

    void RenderLoop()
    {
        while (!window->ShouldClose())
        {
            window->PollEvents();

            if (window->WasResized()) {
                window->ResetResized();
                RecreateSwapChain();
            }

            uint32_t imageIndex = rhiSwapChain->AcquireNextImage();
            uint32_t frame      = rhiSwapChain->GetCurrentFrame();

            UpdateUniformBuffer(frame);

            rhiCommandList->Begin(frame);
            rhiRenderPass->Begin(rhiCommandList, rhiFramebuffer, imageIndex);

            rhiCommandList->SetViewport(0.0f, 0.0f, (float) window->GetWidth(), (float) window->GetHeight());
            rhiCommandList->SetScicors(0, 0, window->GetWidth(), window->GetHeight());

            rhiCommandList->BindPipeline(rhiPipeline);
            rhiCommandList->BindVertexBuffer(vertexBuffer);
            rhiCommandList->BindIndexBuffer(indexBuffer);
            rhiMaterial->Bind(rhiCommandList);

            rhiCommandList->DrawIndexed(static_cast<uint32_t>(rhiModel->GetIndices().size()));

            rhiRenderPass->End(rhiCommandList);
            rhiCommandList->End();

            rhiQueue->Submit(rhiCommandList, rhiSwapChain);
            rhiSwapChain->Present(rhiQueue);
        }
    }

    void UpdateUniformBuffer(uint32_t frameIndex)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = GpuMat4(Mat4::RotationZ(DegToRad(time * 90.0f)).Transpose());
        ubo.view = GpuMat4(camera->GetViewMatrix().Transpose());
        Mat4 proj = camera->GetProjectionMatrix();
        proj.data[5] *= -1.0f;
        ubo.proj = GpuMat4(proj.Transpose());

        uniformBuffers[frameIndex]->Upload(&ubo, sizeof(ubo));
    }

    void RecreateSwapChain()
    {
        window->WaitWhileMinimized();

        rhiDevice->WaitIdle();

        rhiFramebuffer->Destroy();
        rhiSwapChain->Resize(window->GetWidth(), window->GetHeight());
        rhiFramebuffer->Create(rhiDevice, rhiSwapChain, rhiRenderPass);

        camera->SetProjectionMatrix(
            DegToRad(45.0f),
            window->GetWidth() / (float) window->GetHeight(),
            0.1f,
            10.0f);
    }

    void CleanUp()
    {
        rhiDevice->WaitIdle();

        rhi->Destroy(rhiMesh);
        rhi->Destroy(rhiMaterial);
        rhi->Destroy(rhiModel);

        for (IBuffer* uniformBuffer : uniformBuffers)
            rhi->Destroy(uniformBuffer);
        uniformBuffers.clear();

        rhi->Destroy(indexBuffer);
        rhi->Destroy(vertexBuffer);

        rhi->Destroy(rhiSampler);
        rhi->Destroy(rhiTexture);

        rhi->Destroy(rhiCommandList);

        rhi->Destroy(rhiPipeline);
        rhi->Destroy(fragShader);
        rhi->Destroy(vertShader);

        rhi->Destroy(rhiFramebuffer);
        rhi->Destroy(rhiRenderPass);
        rhi->Destroy(rhiSwapChain);
        rhi->Destroy(rhiQueue);

        rhi->Destroy(rhiDevice);
        rhi->Destroy(rhiSurface);
        rhi->Destroy(rhiInstance);

        delete rhi;
        delete camera;

        window->Destroy();
        delete window;
    }

    void InitCamera()
    {
        camera = new Camera();

        camera->eye    = Vector3D(2.0f, 2.0f, 2.0f);
        camera->center = Vector3D(0.0f, 0.0f, 0.0f);
        camera->up     = Vector3D(0.0f, 0.0f, 1.0f);
        camera->SetViewMatrix();

        camera->SetProjectionMatrix(
        DegToRad(45.0f),
        window->GetWidth() / (float) window->GetHeight(),
        0.1f,
        10.0f);
    }

private:
    Window* window;

    Camera* camera;

    IRHI* rhi;

    IInstance*    rhiInstance;
    ISurface*     rhiSurface;
    IDevice*      rhiDevice;
    ISwapChain*   rhiSwapChain;
    IQueue*       rhiQueue;
    IRenderPass*  rhiRenderPass;
    IFramebuffer* rhiFramebuffer;
    IShader*      vertShader;
    IShader*      fragShader;
    IPipeline*    rhiPipeline;
    ICommandList* rhiCommandList;
    IBuffer*      vertexBuffer;
    IBuffer*      indexBuffer;
    ITexture*     rhiTexture;
    ISampler*     rhiSampler;
    IModel*       rhiModel;
    IMesh*        rhiMesh;
    IMaterial*    rhiMaterial;

    std::vector<IBuffer*> uniformBuffers;
};
