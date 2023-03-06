#include "SpriteSheet.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

#include <GL/glew.h>

void SpriteSheet::Load(const char *filename, int spriteWidth, int spriteHeight) {
    m_SpriteWidth = spriteWidth;
    m_SpriteHeight = spriteHeight;

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for (int y = 0; y < height; y += m_SpriteHeight)
    {
        for (int x = 0; x < width; x += m_SpriteWidth)
        {
            int spriteWidth = std::min(m_SpriteWidth, width - x);
            int spriteHeight = std::min(m_SpriteHeight, height - y);
            m_Sprites.emplace_back(x, y, spriteWidth, spriteHeight, width, height);
        }
    }

    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GetTextureID());
}