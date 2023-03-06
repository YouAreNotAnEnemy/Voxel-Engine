#pragma once

#include <glm/glm.hpp>



struct Vec2i {
    int x, y;

    Vec2i() : x(0), y(0) { }
    Vec2i(int _x, int _y) : x(_x), y(_y) {}
};

inline bool operator==(const Vec2i& lhs, const Vec2i& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace std {
    template<>
    struct hash<Vec2i>
    {
        std::size_t operator()(Vec2i const& v) const noexcept
        {
            std::size_t h1 = std::hash<int>{}(v.x);
            std::size_t h2 = std::hash<int>{}(v.y);
            return h1 ^ (h2 << 1);
        }
    };
}

struct Vec3i {
    union {
        int x;
        int r;
    };
    union {
        int y;
        int g;
    };
    union {
        int z;
        int b;
    };
};

struct Vertex {
    glm::vec3 Position;
    glm::vec2 Uv = glm::vec2(-1.0f);
};

struct Face {
    uint32_t x, y, z;
};
