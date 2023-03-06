#pragma once

#include "Logger.h"
#include <chrono>

class Timer {
public:
    Timer() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        m_DurationMs = (float) std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_Start).count() / 1000000.0f;

        LOG_INFO("Took ", m_DurationMs, "ms.");
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    float m_DurationMs = 0;
};