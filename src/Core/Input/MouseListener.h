#pragma once

#include "GLFW/glfw3.h"

#include <unordered_map>

#include <cmath>

#include "glm/glm.hpp"

typedef std::unordered_map<int, bool> ButtonMap;

class MouseListener {
public:
    static uint32_t GetSize() {
        return sizeof(double) * 6 + sizeof(bool) * 2 +
               (sizeof(int) + sizeof(bool)) * buttons.size() +
               (sizeof(int) + sizeof(bool)) * buttonsDown.size() +
               (sizeof(int) + sizeof(bool)) * buttonsHolding.size() +
               (sizeof(int) + sizeof(bool)) * buttonsReleased.size();
    }

    static double scrollX, scrollY;
    static double xPos, yPos, lastX, lastY;

    static bool isDragging, isMoving;

    static ButtonMap buttons;
    static ButtonMap buttonsDown, buttonsHolding, buttonsReleased;

    static void Update(GLFWwindow* window) {
        for (auto& pair : buttons)
        {
            int key = pair.first; bool value = pair.second;

            if (value) {
                /* Key is currently down */
                if (!buttonsHolding[key]) {
                    /* Key isn't marked as holding
                     * -> First Frame it's down
                     * -> Second Frame -> mark as holding */

                    if (buttonsDown[key]) {
                        /* Key was down last Frame -> holding */
                        buttonsDown[key] = false;
                        buttonsHolding[key] = true;
                    } else {
                        buttonsDown[key] = true;
                        buttonsHolding[key] = false; //TODO: Remove this Line
                    }
                }
            }
            else {
                /* Key isn't pressed this Frame -> Check if it got released */
                if ((buttonsDown[key] || buttonsHolding[key]) && !buttonsReleased[key]) {
                    buttonsReleased[key] = true;
                } else {
                    buttonsReleased[key] = false;
                }

                buttonsDown[key] = false;
                buttonsHolding[key] = false;
            }
        }
        lastX = xPos;
        lastY = yPos;

        glfwGetCursorPos(window, &xPos, &yPos);

        isMoving = std::max(xPos, lastX) - std::min(xPos, lastX) > 0 || std::max(yPos, lastY) - std::min(yPos, lastY) > 0;
        isDragging = isMoving && buttonsHolding[0];
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
//            lastX = xPos;
//            lastY = yPos;
//
//            MouseListener::xPos = xpos;
//            MouseListener::yPos = ypos;
//
//            isMoving = std::max(xPos, lastX) - std::min(xPos, lastX) > 0 || std::max(yPos, lastY) - std::min(yPos, lastY) > 0;
//            isDragging = buttonsHolding[0];
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
        buttons[button] = action != GLFW_RELEASE;
        if (action == GLFW_RELEASE) isDragging = false;
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
        scrollX = xoffset;
        scrollY = yoffset;
    }

};
