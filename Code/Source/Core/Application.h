#pragma once
#include "Core/Window/Window.h"
#include "RHI/IDevice.h"
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
        rhiSurface->Create();
        rhiDevice->Create();
        rhiSwapChain->Create();
        
        InitCamera();
        //Image views
        rhiRenderPass->Create();
        //Descriptor set
        rhiPipeline->Create();
        rhiCommandList->Create();
        //ColorResources
        //DepthResources
        //Framebuffers
        rhiTexture->Create();
        
        LoadModel();
        
        vertexBuffer->Create();
        indexBuffer->Create();
        
        //createUniformBuffers
        //createDescriptorPool
        //createDescriptorSets
        //createCommandBuffers
        //createSyncObjects
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
    
    void LoadModel()
    {
        
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
   	Editor app;
    
   	try 
       {
           app.run();
       } catch (const std::exception& e) {
           std::cerr << e.what() << std::endl;
           return EXIT_FAILURE;
       }
   
       return EXIT_SUCCESS;
}