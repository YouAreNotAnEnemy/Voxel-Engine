#include "World.h"

#include "GL/glew.h"
#include "Util/Logger.h"

#include "Core/Application.h"
#include "Util/Timer.h"

World* World::s_Instance = nullptr;

void World::Render() {
    for (auto & activeChunk : activeChunks) {
        activeChunk.second->Render();
    }
}

void World::LoadSpawn() {
    chunkLoadQueue.reserve(WORLD_SIZE * WORLD_SIZE);

    Chunk::s_SpriteSheet.Load("assets/Textures/atlas.png", 16, 16);
    s_Instance->chunkManager = std::thread(WorkOnQueue);
}

void World::UnloadSpawn() {
    for (auto & activeChunk : activeChunks) {
        delete activeChunk.second;
    }

    activeChunks.clear();
}

World* World::Create() {
    s_Instance = new World();
    return s_Instance;
}

void World::WorkOnQueue() {
    LOG_DEBUG("Starting ChunkWorker Thread...");

    auto& world = GetWorld();
    world.playerPosition = Application::Get().GetCamera().m_Position;

    srand(time(NULL));
    Chunk::s_FastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    Chunk::s_FastNoise.SetSeed(rand());
    Chunk::s_FastNoise.SetFractalType(FastNoiseLite::FractalType_DomainWarpIndependent);
    Chunk::s_FastNoise.SetFractalOctaves(3.0f);
    Chunk::s_FastNoise.SetFractalGain(120.0f);
    Chunk::s_FastNoise.SetFractalLacunarity(1.0f);
    Chunk::s_FastNoise.SetFrequency(0.01f);


    for (int x = 0 - (HALF_WORLD_SIZE); x < HALF_WORLD_SIZE; ++x) {
        for (int z = 0 - (HALF_WORLD_SIZE); z < HALF_WORLD_SIZE; ++z) {
            auto* chunk = new Chunk();
            chunk->SetPos({x, z});

            world.activeChunks[{x, z}] = chunk;
            world.chunkLoadQueue.push_back(chunk);
        }
    }

    for (const auto &chunk: s_Instance->chunkLoadQueue) {
        chunk->SetAllSolid();
    }

    {
        Timer timer;

        for (const auto &chunk: s_Instance->chunkLoadQueue) {
            chunk->GenerateMesh();
        }
    }

    LOG_DEBUG("Finished generating all Chunks");
}

void World::Update() {
    playerPosition = Application::Get().GetCamera().m_Position;
}






