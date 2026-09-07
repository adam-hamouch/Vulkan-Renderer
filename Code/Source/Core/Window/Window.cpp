#include "../Include/Core/Window/Window.h"

void Window::Create(const char* name, uint32_t width, uint32_t height)
{
    this->width  = width;
    this->height = height;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void Window::Destroy()
{
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}

void Window::PollEvents()
{
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->width  = width;
    win->height = height;
}