#pragma once

#include "Window.h"

#include "Render/Camera.h"
#include "Render/Shader.h"
#include "Render/ImGui/ImGuiLayer.h"

#include "Voxeling/World.h"

class Application {
public:
    static Application* Create();

    static Application& Get() { return *s_Instance; }

    Camera& GetCamera() { return camera; }

    void Run();

    void SubmitToMainThread(const std::function<void()>& func);

private:
    void Start();

    void Update();

    void ExecuteMainThreadQueue();

    Window window;
    Camera camera;

    Shader shader;
    ImGuiLayer imGuiLayer;

    World* world;

    float deltaTime = 0.0f;

    std::vector<std::function<void()>> mainThreadQueue;
    std::mutex mainThreadQueueMutex;

private:
    static Application* s_Instance;
};
