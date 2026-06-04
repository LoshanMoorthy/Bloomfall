#pragma once

constexpr int MAP_WIDTH = 10;
constexpr int MAP_HEIGHT = 10;

struct World {
    int height_map[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 4, 1, 1, 1, 1, 1, 1},
        {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 3, 1, 1, 1, 1, 1},
        {1, 1, 1, 2, 3, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 3, 3, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 3, 1}
    };

    int height_at(int x, int y) const { return height_map[y][x]; }
};
