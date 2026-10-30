#pragma once

#include "Core/Window/Window.h"
#include "Core/Scene.h"
#include "LowRenderer/Renderer.h"
#include "RHI/VulkanUtils.h"

class Application
{
public:
    void run()
    {
        window.Create("Vulkan Renderer", WIDTH, HEIGHT);
        renderer.Init(window);
        scene.Load(renderer, window);

        while (!window.ShouldClose())
        {
            window.PollEvents();

            if (window.WasResized())
            {
                window.ResetResized();
                renderer.Resize(window);
                scene.Resize(window);
            }

            scene.Update();
            renderer.Draw(window, scene);
        }

        renderer.WaitIdle();
        scene.Unload(renderer);
        renderer.Destroy();
        window.Destroy();
    }

private:
    Window   window;
    Renderer renderer;
    Scene    scene;
};
