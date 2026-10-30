#include "Core/Scene.h"

#include "Core/Window/Window.h"
#include "LowRenderer/Renderer.h"
#include "RHI/IMesh.h"
#include "RHI/IMaterial.h"
#include "Resources/ResourcesManager.h"

#include <chrono>

void Scene::Load(Renderer& renderer, Window& window)
{
    camera.InitBaseCamera(&window);

    texture = renderer.GetResourcesManager()->Load<ITexture>("../../Ressources/Textures/viking_room.png");

    material = renderer.GetRHI()->CreateMaterial();
    material->Create(renderer.GetDevice(), {texture, nullptr, renderer.GetSampler(), renderer.GetPipeline(), renderer.GetUniformBuffers()});

    mesh = renderer.GetRHI()->CreateMesh();
    mesh->Create(renderer.GetDevice(), "../../Ressources/Models/viking_room.obj", renderer.GetResourcesManager());
}

void Scene::Update()
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    modelMatrix = Mat4::RotationZ(DegToRad(time * 90.0f));
}

void Scene::Resize(Window& window)
{
    camera.SetProjectionMatrix(
        DegToRad(45.0f),
        window.GetWidth() / (float) window.GetHeight(),
        0.1f,
        10.0f);
}

void Scene::Unload(Renderer& renderer)
{
    renderer.GetRHI()->Destroy(mesh);
    renderer.GetRHI()->Destroy(material);
    texture.reset();
}
