#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

struct WindowSettings {
    int Width, Height;
    const char* Title;
};

class Window {
public:
    Window() = default;

    void Create(int width, int height, const char* title, GLFWwindow* share = nullptr);

    bool ShouldClose() { return glfwWindowShouldClose(m_Window); }

    void SwapBuffers() { glfwSwapBuffers(m_Window); }

    void PollEvents() { glfwPollEvents(); }

    GLFWwindow* GetGLFWwindow() { return m_Window; }

private:

    void InitGLFW(GLFWwindow* share = nullptr);

    void InitGlew();

    WindowSettings m_Settings;

    GLFWwindow* m_Window;
};
