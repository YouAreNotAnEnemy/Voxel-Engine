#include "Window.h"

#include <iostream>

#include "GL/glew.h"

#include "Util/Logger.h"
#include <thread>

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

void Window::Create(int width, int height, const char *title, GLFWwindow* share) {
    m_Settings = WindowSettings {
        .Width = width,
        .Height = height,
        .Title = title
    };

    InitGLFW(share);
    InitGlew();
}

void Window::InitGLFW(GLFWwindow* share) {
    /* Initialize the Library */
    if (!glfwInit()) {
        LOG_WARN("Failed to init glfw");
        return;
    }

    /* Setting the Window Hints (Settings) */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    m_Window = glfwCreateWindow(m_Settings.Width, m_Settings.Height, m_Settings.Title, nullptr, share);
    if (!m_Window) {
        glfwTerminate();
        return;
    }

    glfwSetWindowAspectRatio(m_Window, 16, 9);

    glfwSetWindowUserPointer(m_Window, &m_Settings);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);

        WindowSettings& settings = *(WindowSettings*) glfwGetWindowUserPointer(window);
        settings.Width = width;
        settings.Height = height;
    });

    glfwMakeContextCurrent(m_Window);
    LOG_INFO("Made Context current on Thread: ", std::this_thread::get_id());
}

void Window::InitGlew() {
    if (glewInit() == -1) {
        LOG_WARN("Failed to init glew");
        return;
    }

    auto buffer = glGetString(GL_VERSION);
    LOG_DEBUG(buffer);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(glDebugOutput, nullptr);
}

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam){

    std::string sourceStr;
    std::string typeStr;
    std::string severityStr;

    switch (source) {
        case GL_DEBUG_SOURCE_API:               sourceStr = "API";                             break;
        case GL_DEBUG_SOURCE_APPLICATION:       sourceStr = "Application";                     break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   sourceStr = "Shader Compiler";                 break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       sourceStr = "Third Party";                     break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     sourceStr = "Window System";                   break;
        case GL_DEBUG_SOURCE_OTHER:             sourceStr = "Other";                           break;
        default:                                sourceStr = std::to_string(source);         break;}

    switch (type) {
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance";                        break;
        case GL_DEBUG_TYPE_ERROR:               typeStr = "Error";                              break;
        case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker";                             break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior";                break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group";                          break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group";                         break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability";                        break;
        case GL_DEBUG_TYPE_OTHER:               typeStr = "Other";                              break;
        default:                                typeStr = std::to_string(type);             break;}

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            severityStr = "High";                           break;
        case GL_DEBUG_SEVERITY_MEDIUM:          severityStr = "Medium";                         break;
        case GL_DEBUG_SEVERITY_LOW:             severityStr = "Low";                            break;
        default:                                severityStr = std::to_string(severity);     break;
    }

    LOG_ERROR("[OpenGL - ", severityStr, "] Source: ", sourceStr);
    LOG_ERROR("[OpenGL - ", severityStr, "] Type: ", typeStr);
    LOG_ERROR("[OpenGL - ", severityStr, "] ID: ", id);
    LOG_ERROR("[OpenGL - ", severityStr, "] Hint: ", message);

    __debugbreak();
}
