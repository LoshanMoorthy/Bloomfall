#include "world.h"
#include "external/FastNoiseLite.h"

// TODO: noise
void World::generate() {
    const int WALL_H = 4;
    const int FLOOR_Y = 0;

    for (int i = 0; i < WORLD_X*WORLD_Y*WORLD_Z; i++)
        voxels[i] = 0;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.08f);

    for (int z = 0; z < WORLD_Z; z++) {
        for (int x = 0; x < WORLD_X; x++) {
            // always a floor
            set(x, FLOOR_Y, z, 1);

            // 2D noise wall or open
            float v = noise.GetNoise((float)x, (float)z);
            bool wall = v > 0.2f;
            if (wall)
                for (int y = FLOOR_Y+1; y <= WALL_H; y++)
                    set(x, y, z, 1);
        }
    }

    int cx = WORLD_X/2, cz = WORLD_Z/2;
    for (int z = cz-5; z <= cz+5; z++)
        for (int x = cx-5; x <= cx+5; x++)
            for (int y = FLOOR_Y+1; y <= WALL_H; y++)
                set(x, y, z, 0);
}
