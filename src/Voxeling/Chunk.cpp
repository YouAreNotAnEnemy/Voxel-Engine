#include "Chunk.h"

#include "GL/glew.h"
#include "Util/Logger.h"
#include "Core/Application.h"

#include <chrono>

#include "World.h"
#include "Util/Timer.h"

SpriteSheet Chunk::s_SpriteSheet = SpriteSheet();
FastNoiseLite Chunk::s_FastNoise = FastNoiseLite();

void Chunk::GenerateMesh() {
    auto& world = World::GetWorld();

    for (int chunkX = 0; chunkX < CHUNK_WIDTH; ++chunkX) {
        for (int chunkY = 0; chunkY < CHUNK_HEIGHT; ++chunkY) {
            for (int chunkZ = 0; chunkZ < CHUNK_WIDTH; ++chunkZ) {
                auto id = m_Voxels[chunkX][chunkY][chunkZ];
                if (id == 0) continue;

                auto worldPos = ChunkPosToWorldPos(chunkX, chunkY, chunkZ);

                /* West Side | Negative X direction */
                {
                    if (chunkX == 0) {
                        /* Check if in that direction is a block in the next chunk */
                        if (m_ChunkPosition.x - 1 >= 0 - HALF_WORLD_SIZE &&
                            world.activeChunks[{m_ChunkPosition.x - 1, m_ChunkPosition.y}]->m_Voxels[CHUNK_WIDTH - 1][chunkY][chunkZ] == 0)
                                AddWestSide(worldPos, id);

                    }
                    if (chunkX > 0) if (m_Voxels[chunkX - 1][chunkY][chunkZ] == 0) AddWestSide(worldPos, id);
                }

                /* East Side | Positive X direction */
                {
                    if (chunkX == CHUNK_WIDTH - 1) {

                        if (m_ChunkPosition.x + 1 < HALF_WORLD_SIZE &&
                            world.activeChunks[{m_ChunkPosition.x + 1, m_ChunkPosition.y}]->m_Voxels[0][chunkY][chunkZ] == 0)
                                AddEastSide(worldPos, id);
                    }
                    if (chunkX < CHUNK_WIDTH - 1) if (m_Voxels[chunkX + 1][chunkY][chunkZ] == 0) AddEastSide(worldPos, id);
                }

                /* North Side | Negative Z direction */
                {
                    if (chunkZ == 0) {

                        if (m_ChunkPosition.y - 1 >= 0 - HALF_WORLD_SIZE &&
                            world.activeChunks[{m_ChunkPosition.x, m_ChunkPosition.y - 1}]->m_Voxels[chunkX][chunkY][CHUNK_WIDTH - 1] == 0)
                                AddNorthSide(worldPos, id);
                    }
                    if (chunkZ > 0) if (m_Voxels[chunkX][chunkY][chunkZ - 1] == 0) AddNorthSide(worldPos, id);
                }


                /* South Side | Positive Z direction */
                {
                    if (chunkZ == CHUNK_WIDTH - 1) {

                        if (m_ChunkPosition.y + 1 < HALF_WORLD_SIZE &&
                            world.activeChunks[{m_ChunkPosition.x, m_ChunkPosition.y + 1}]->m_Voxels[chunkX][chunkY][0] == 0)
                                AddSouthSide(worldPos, id);
                    }

                    if (chunkZ < CHUNK_WIDTH - 1) if (m_Voxels[chunkX][chunkY][chunkZ + 1] == 0) AddSouthSide(worldPos, id);
                }


                /* Buttom Side | Negative Y direction */
                {
                    if (chunkY == 0) AddButtomSide(worldPos, id);
                    if (chunkY > 0) if (m_Voxels[chunkX][chunkY - 1][chunkZ] == 0) AddButtomSide(worldPos, id);
                }

                /* Top Side | Positive Y direction */
                {
                    if (chunkY == CHUNK_HEIGHT - 1) AddTopSide(worldPos, id);
                    if (chunkY < CHUNK_HEIGHT - 1) if (m_Voxels[chunkX][chunkY + 1][chunkZ] == 0) AddTopSide(worldPos, id);
                }
            }
        }
    }

    Application::Get().SubmitToMainThread([this]()
    {
        Chunk::CreateBuffers(this);
    });
}

void Chunk::CreateBuffers(Chunk* chunk) {
    glGenVertexArrays(1, &chunk->m_Vao);
    glBindVertexArray(chunk->m_Vao);

    glGenBuffers(1, &chunk->m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices[0]) * chunk->m_Vertices.size(), chunk->m_Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &chunk->m_Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->m_Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Faces[0]) * chunk->m_Faces.size(), chunk->m_Faces.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    /* Clear Vertex and Index data after pushing to GPU to safe Memory */
    chunk->m_IndicesCount = chunk->m_Faces.size() * 3;

    chunk->m_Vertices.clear();
    chunk->m_Vertices.shrink_to_fit();

    chunk->m_Faces.clear();
    chunk->m_Faces.shrink_to_fit();

    chunk->m_IsGenerated = true;
}

void Chunk::AddNorthSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(id);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z },
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z},
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 2});
    m_Faces.push_back({ currentIndex, currentIndex + 1, currentIndex + 3});

    m_Index += 4;
}

void Chunk::AddEastSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(id);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z },
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 2});
    m_Faces.push_back({ currentIndex, currentIndex + 1, currentIndex + 3});

    m_Index += 4;
}

void Chunk::AddSouthSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(id);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 2, currentIndex + 3});
    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 1});

    m_Index += 4;
}

void Chunk::AddWestSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(id);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z},
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 2, currentIndex + 3});
    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 1});

    m_Index += 4;
}

void Chunk::AddTopSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(7);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y, worldPos.z },
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 2});
    m_Faces.push_back({ currentIndex, currentIndex + 1, currentIndex + 3});

    m_Index += 4;
}

void Chunk::AddButtomSide(Vec3i worldPos, int id) {
    uint32_t currentIndex = m_Index;
    auto& sprite = s_SpriteSheet.GetSprite(5);

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z + 1 },
            .Uv = glm::vec2(sprite.u2, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x + 1, worldPos.y - 1, worldPos.z },
            .Uv = glm::vec2(sprite.u2, sprite.v2)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z + 1},
            .Uv = glm::vec2(sprite.u1, sprite.v1)
    });

    m_Vertices.push_back(Vertex {
            .Position = { worldPos.x, worldPos.y - 1, worldPos.z},
            .Uv = glm::vec2(sprite.u1, sprite.v2)
    });

    m_Faces.push_back({ currentIndex, currentIndex + 2, currentIndex + 3});
    m_Faces.push_back({ currentIndex, currentIndex + 3, currentIndex + 1});

    m_Index += 4;
}

void Chunk::Render() {
    if (m_IsGenerated) {
        glBindVertexArray(m_Vao);
        glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);
    }
}

void Chunk::SetAllSolid() {
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            auto worldPos = ChunkPosToWorldPos(x, 0, z);
            int height = 40 + s_FastNoise.GetNoise((float) worldPos.x, (float) worldPos.z) * 30;

            for (int y = 0; y < height; ++y) {
                m_Voxels[x][y][z] = 6;
            }

            for (int y = height; y < CHUNK_HEIGHT; ++y) {
                m_Voxels[x][y][z] = 0;
            }
        }
    }
}

Vec3i Chunk::ChunkPosToWorldPos(int x, int y, int z) const {
    Vec3i pos {};

    pos.x = (int) m_ChunkPosition.x * CHUNK_WIDTH + x;
    pos.y = y;
    pos.z = (int) m_ChunkPosition.y * CHUNK_WIDTH + z;

    return pos;
}

Vec3i Chunk::WorldPosToChunkPos(int x, int y, int z) const {
    Vec3i pos {};

    int chunkX = floor(x / CHUNK_WIDTH);
    int chunkZ = floor(z / CHUNK_WIDTH);

    pos.x = floor(x - CHUNK_WIDTH * chunkX);
    pos.y = y;
    pos.z = floor(z - CHUNK_WIDTH * chunkZ);

    return pos;
}

