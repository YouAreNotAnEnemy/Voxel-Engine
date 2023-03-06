#pragma once


#include "GLFW/glfw3.h"
#include "KeyListener.h"
#include "MouseListener.h"
#include "KeyCodes.h"

#include <iostream>

class Input {
public:
    static void PrintMemory() {
        std::cout << "Input Size: " << KeyListener::GetSize() + MouseListener::GetSize() << std::endl;
    }

    static void SetWindow(GLFWwindow* window) {
        s_Window = window;

        glfwSetKeyCallback(window, KeyListener::key_callback);

        glfwSetMouseButtonCallback(window, MouseListener::mouse_button_callback);
        glfwSetCursorPosCallback(window, MouseListener::cursor_position_callback);
        glfwSetScrollCallback(window, MouseListener::scroll_callback);
    }

    static void Update() {
        KeyListener::Update();
        MouseListener::Update(s_Window);
    }

    static void HideCursor() { glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }

    static void ShowCursor() { glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

    static bool GetKey(KeyCode key) { return KeyListener::keyMap[key]; }

    static bool IsKeyDown(KeyCode key) { return KeyListener::keysDown[key]; }

    static bool IsKeyHolding(KeyCode key) { return KeyListener::keysHolding[key]; }

    static bool GotKeyReleased(KeyCode key) { return KeyListener::keysReleased[key]; }

    static bool GetMouseButton(int button) { return MouseListener::buttons[button]; }

    static bool IsMouseButtonDown(int button) { return MouseListener::buttonsDown[button]; }

    static bool IsMouseButtonHolding(int button) { return MouseListener::buttonsHolding[button]; }

    static bool GotMouseButtonReleased(int button) { return MouseListener::buttonsReleased[button]; }

    static bool IsMouseDragging() { return MouseListener::isDragging; }

    static bool IsMouseMoving() { return MouseListener::isMoving; }

    static void SetMousePosition(double xPos, double yPos) { glfwSetCursorPos(s_Window, xPos, yPos); }

    static glm::vec2 GetMousePosition() {
        double x, y;
        glfwGetCursorPos(s_Window, &x, &y);
        return {x, y};
    }

    static glm::vec2 GetWindowSize() {
        int x, y;
        glfwGetWindowSize(s_Window, &x, &y);
        return {x, y};
    }

private:
    static GLFWwindow* s_Window;
};
