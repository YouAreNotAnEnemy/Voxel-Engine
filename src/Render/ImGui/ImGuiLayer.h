#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"

class ImGuiLayer
{
private:
    GLFWwindow* m_WindowPtr;

public:
    static ImFont* RegularFont;
    static ImFont* BoldFont;


public:
    ImGuiLayer();

    void Init(GLFWwindow* windowPtr);

    void Prepare();

    void Render();

    void Terminate();

private:
    void SetStyle();

    void PrepareDockspace();
};
