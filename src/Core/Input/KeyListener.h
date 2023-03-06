#pragma once

#include <unordered_map>
#include <algorithm>

#include "GLFW/glfw3.h"

typedef std::unordered_map<uint16_t, bool> KeyMap;

class KeyListener {
public:
    static KeyMap keyMap;
    static KeyMap keysDown, keysHolding, keysReleased;

    static uint32_t GetSize() {
        return  (sizeof(uint16_t) + sizeof(bool)) * keyMap.size() +
                (sizeof(uint16_t) + sizeof(bool)) * keysDown.size() +
                (sizeof(uint16_t) + sizeof(bool)) * keysHolding.size() +
                (sizeof(uint16_t) + sizeof(bool)) * keysReleased.size();
    }

    static void Update() {
        for (auto& pair : keyMap)
        {
            uint16_t key = pair.first; bool value = pair.second;

            if (value) {
                /* Key is currently down */
                if (!keysHolding[key]) {
                    /* Key isn't marked as holding
                     * -> First Frame it's down
                     * -> Second Frame -> mark as holding */

                    if (keysDown[key]) {
                        /* Key was down last Frame -> holding */
                        keysDown[key] = false;
                        keysHolding[key] = true;
                    } else {
                        keysDown[key] = true;
                        keysHolding[key] = false; //TODO: Remove this Line
                    }
                }
            }
            else {
                /* Key isn't pressed this Frame -> Check if it got released */
                if ((keysDown[key] || keysHolding[key]) && !keysReleased[key]) {
                    keysReleased[key] = true;
                } else {
                    keysReleased[key] = false;
                }

                keysDown[key] = false;
                keysHolding[key] = false;
            }
        }
    }

    static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {
        keyMap[key] = action != GLFW_RELEASE;
    }
};
