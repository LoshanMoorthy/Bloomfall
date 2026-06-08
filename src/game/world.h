#pragma once

#include <cstdint>

constexpr int WORLD_X = 32;
constexpr int WORLD_Y = 16;
constexpr int WORLD_Z = 32;

struct World {
    uint8_t voxels[WORLD_X * WORLD_Y * WORLD_Z];

    // flatten 3D coords -> 1D index
    int index(int x, int y, int z) const {
        return x + WORLD_X * (y + WORLD_Y * z);
    }

    bool in_bounds(int x, int y, int z) const {
        return x >= 0 && x < WORLD_X &&
               y >= 0 && y < WORLD_Y &&
               z >= 0 && z < WORLD_Z;
    }

    uint8_t get(int x, int y, int z) const {
        if (!in_bounds(x, y, z)) return 0;
        return voxels[index(x, y, z)];
    }

    void set(int x, int y, int z, uint8_t v) {
        if (in_bounds(x, y, z)) voxels[index(x, y, z)] = v;
    }

    bool is_solid(int x, int y, int z) const {
        return get(x, y, z) != 0;
    }

    void generate();
};
