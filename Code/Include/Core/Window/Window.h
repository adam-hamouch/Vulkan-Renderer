#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>

class Window
{
public:
    ~Window() { Destroy(); }

    void Create(const char* name, uint32_t width, uint32_t height);
    void Destroy();
    void PollEvents();
    bool ShouldClose() const;

    // Bloque tant que la fenetre est minimisee (framebuffer de taille 0)
    void WaitWhileMinimized();

    bool WasResized()   const { return resized; }
    void ResetResized()       { resized = false; }

    GLFWwindow* GetHandle() const { return window; }
    uint32_t    GetWidth()  const { return width; }
    uint32_t    GetHeight() const { return height; }

private:
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window  = nullptr;
    uint32_t    width   = 0;
    uint32_t    height  = 0;
    bool        resized = false;
};