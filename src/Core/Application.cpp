#include "Application.h"
#include "GL/glew.h"

#include "Input/Input.h"
#include "Util/Logger.h"

#include <unordered_map>

Application* Application::s_Instance = nullptr;

Application *Application::Create() {
    s_Instance = new Application();
    return s_Instance;
}

void Application::Run() {
    Start();
    Update();
}

void Application::Start() {
    window.Create(1920, 1080, "VoxelEngine");
    camera = Camera(glm::vec3(0.0f, 800.0f, 0.0f));

    Input::SetWindow(window.GetGLFWwindow());
    imGuiLayer.Init(window.GetGLFWwindow());

    shader.Create("assets/Shaders/Cube.glsl");
    shader.Bind();

    world = World::Create();
    world->LoadSpawn();

    /* Init */
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Application::Update() {
    while (!window.ShouldClose()) {
        auto start = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

        ExecuteMainThreadQueue();

        imGuiLayer.Prepare();

        Input::Update();

        camera.Update(deltaTime);
        world->Update();

        {
            ImGui::Begin("Settings");
            ImGui::DragFloat("Camera Speed", &camera.m_Speed);
            ImGui::End();
        }

        shader.UploadMat4f("u_view", camera.GetViewMatrix());
        shader.UploadMat4f("u_proj", camera.GetProjectionMatrix());

        world->Render();

        imGuiLayer.Render();

        window.SwapBuffers();
        window.PollEvents();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        auto micro = (float) duration.count() / 1000.0f;
        auto ms = micro / 1000.0f;
        auto seconds = ms / 1000.0f;

        deltaTime = seconds;
    }

    world->UnloadSpawn();
    glfwTerminate();
}

void Application::SubmitToMainThread(const std::function<void()> &func) {
    std::scoped_lock<std::mutex> lock(mainThreadQueueMutex);
    mainThreadQueue.emplace_back(func);
}

void Application::ExecuteMainThreadQueue() {
    std::scoped_lock<std::mutex> lock(mainThreadQueueMutex);

    for (auto& func : mainThreadQueue) {
        func();
    }

    mainThreadQueue.clear();
}