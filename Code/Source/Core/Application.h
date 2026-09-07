#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

#include "Core/Window/Window.h"
#include "RHI/RHIUtils.h"
#include "RHI/RHI.h"
#include "RHI/VkRHI.h"

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
        rhi = new VulkanRHI();

        rhiInstance    = rhi->CreateInstance();
        rhiSurface     = rhi->CreateSurface();
        rhiDevice      = rhi->CreateDevice();
        rhiSwapChain   = rhi->CreateSwapChain();
        rhiQueue       = rhi->CreateQueue();
        rhiRenderPass  = rhi->CreateRenderPass();
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
    }
    
    void InitRenderingEngine()
    {
        rhiInstance->Create("VulkanRenderer");
        rhiSurface->Create(rhiInstance, window->GetHandle());
        rhiDevice->Create(rhiInstance, rhiSurface);
        rhiSwapChain->Create(rhiDevice, rhiSurface, window->GetHandle());
        rhiQueue->Create(rhiDevice, QueueType::Graphics);

        InitCamera();

        RenderPassDesc renderPassDesc;
        renderPassDesc.attachments = {
            {AttachmentFormat::BGRA8,   LoadOp::Clear,    StoreOp::DontCare, false, false, 4},
            {AttachmentFormat::Depth32, LoadOp::Clear,    StoreOp::DontCare, true,  false, 4},
            {AttachmentFormat::BGRA8,   LoadOp::DontCare, StoreOp::Store,    false, true,  1}
        };
        renderPassDesc.subPasses = { {{0}, {}, 1} };
        rhiRenderPass->Create(rhiDevice, renderPassDesc);

        ShaderDesc vertDesc{"Shaders/vert.spv", ShaderStage::Vertex};
        ShaderDesc fragDesc{"Shaders/frag.spv", ShaderStage::Fragment};
        vertShader->Create(rhiDevice, vertDesc);
        fragShader->Create(rhiDevice, fragDesc);

        PipelineDesc pipelineDesc;
        pipelineDesc.shaders     = {vertShader, fragShader};
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

        rhiTexture->Load("Textures/viking_room.png");

        SamplerDesc samplerDesc;
        rhiSampler->Create(rhiDevice, samplerDesc);

        rhiMaterial->Create({rhiTexture, nullptr, rhiSampler, rhiPipeline});

        LoadModel("../../Ressources/Models/viking_room.obj");

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

        // TODO: createUniformBuffers
        // TODO: createDescriptorPool
        // TODO: createDescriptorSets
        // TODO: createCommandBuffers
        // TODO: createSyncObjects
    }
    
    void RenderLoop()
    {
    }

    void CleanUp()
    {
        rhiInstance->Destroy();
    }
    
    void InitCamera()
    {
        camera = new Camera();
        
        camera->eye    = Vector3D(2.0f, 2.0f, 2.0f);
        camera->center = Vector3D(0.0f, 0.0f, 0.0f);
        camera->up     = Vector3D(0.0f, 0.0f, 1.0f);
        camera->SetViewMatrix();
        
        camera->SetProjectionMatrix(
        45.0f * (M_PI / 180.0f), 
        swapChainExtent.width / (float)swapChainExtent.height,
        0.1f,
        10.0f);
    }
    
    void LoadModel(const char* path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path))
            throw std::runtime_error(err);

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
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
};

int main() 
{
   	Application app;
    
   	try 
       {
           app.run();
       } catch (const std::exception& e) {
           std::cerr << e.what() << std::endl;
           return EXIT_FAILURE;
       }
   
       return EXIT_SUCCESS;
}