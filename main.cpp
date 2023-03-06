#include "Core/Application.h"



int main() {
    auto* app = Application::Create();

    app->Run();

    delete app;

    return 0;
}
