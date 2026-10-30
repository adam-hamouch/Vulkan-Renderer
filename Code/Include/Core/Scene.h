#pragma once

#include "LowRenderer/Camera.h"

#include <memory>

class Renderer;
class Window;
class IMesh;
class IMaterial;
class ITexture;

class Scene
{
public:
    void Load(Renderer& renderer, Window& window);
    void Update();
    void Resize(Window& window);
    void Unload(Renderer& renderer);

    Camera camera;
    Mat4   modelMatrix = Mat4::Identity();

    IMesh*     mesh     = nullptr;
    IMaterial* material = nullptr;
    std::shared_ptr<ITexture> texture;
};
