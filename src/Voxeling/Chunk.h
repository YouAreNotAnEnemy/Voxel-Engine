#pragma once

#include <vector>
#include "Render/Defines.h"
#include "Render/SpriteSheet.h"
#include "FastNoise.h"

#define WORLD_SIZE 64
#define HALF_WORLD_SIZE WORLD_SIZE / 2

#define CHUNK_WIDTH 32
#define CHUNK_HEIGHT 128

class Chunk {
public:
    Chunk() = default;

private:
    void Render();

    void SetPos(Vec2i pos) { m_ChunkPosition = pos; }

    void SetAllSolid();

    void GenerateMesh();

    static void CreateBuffers(Chunk* chunk);

    void AddNorthSide(Vec3i worldPos, int id);
    void AddEastSide(Vec3i worldPos, int id);
    void AddSouthSide(Vec3i worldPos, int id);
    void AddWestSide(Vec3i worldPos, int id);
    void AddTopSide(Vec3i worldPos, int id);
    void AddButtomSide(Vec3i worldPos, int id);

    Vec3i ChunkPosToWorldPos(int x, int y, int z) const;
    Vec3i WorldPosToChunkPos(int x, int y, int z) const;

private:
    friend class World;

    static SpriteSheet s_SpriteSheet;
    static FastNoiseLite s_FastNoise;

    Vec2i m_ChunkPosition;

    /* Stores every Voxel as an ID from 0 to 255
     * Only disadvantage, we store air (ID: 0) */
    uint8_t m_Voxels[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

    /* TODO: Doesnt quite belong here */
    uint32_t m_Vao = 0, m_Vbo = 0, m_Ebo = 0;

    /* TODO: Dont really need them here, because we delete the MeshData after pushing it to the GPU */
    std::vector<Vertex> m_Vertices;
    std::vector<Face> m_Faces;

    /* To keep track of the Indices */
    uint32_t m_Index = 0;

    /* How many Indices we have. We store that because we nowhere store Vertex Data, therefore no way of calculating them */
    uint32_t m_IndicesCount = 0;

    bool m_IsGenerated = false;
};
