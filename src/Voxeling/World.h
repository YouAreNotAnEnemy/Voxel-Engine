#pragma once

#include "Chunk.h"

#include <unordered_map>
#include <thread>
#include <mutex>

/* Manages Chunks and their generation, loading and unloading */
class World {
public:
    static World* Create();

    static World& GetWorld() { return *s_Instance; }

    void Update();

    void Render();

    /* Function to load the Chunks around the Spawn aka x/z 0.0 */
    void LoadSpawn();

    /* Because we need to Unload Chunks we need this function */
    void UnloadSpawn();

    static void WorkOnQueue();

private:
    World() = default;
    static World* s_Instance;

    glm::vec3 playerPosition;

    std::unordered_map<Vec2i, Chunk*> activeChunks;

    std::vector<Chunk*> chunkLoadQueue;
    std::thread chunkManager;

    std::vector<Chunk*> loadedChunksQueue;
    std::mutex loadedChunksMutex;

    friend class Chunk;
};
