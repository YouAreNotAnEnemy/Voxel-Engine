#pragma once

#include <vector>
#include <iostream>

struct Sprite
{
    int x, y, width, height;
    float u1, v1, u2, v2;
    Sprite(int _x, int _y, int _width, int _height, int sheetWidth, int sheetHeight) :
            x(_x), y(_y), width(_width), height(_height)
    {
        u1 = (float)x / sheetWidth;
        v1 = (float)y / sheetHeight;
        u2 = (float)(x + width) / sheetWidth;
        v2 = (float)(y + height) / sheetHeight;
    }
};

class SpriteSheet
{
public:
    SpriteSheet() = default;

    void Load(const char* filename, int spriteWidth, int spriteHeight);

    const Sprite& GetSprite(int index) const { return m_Sprites[index - 1]; }
    int GetSpriteCount() const { return m_Sprites.size(); }
    uint32_t GetTextureID() const { return m_TextureID; }

private:
    std::vector<Sprite> m_Sprites;
    int m_SpriteWidth, m_SpriteHeight;
    uint32_t m_TextureID;
};
