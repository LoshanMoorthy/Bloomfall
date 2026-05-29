#pragma once

#include <SDL2/SDL.h>

#include "camera.h"

void render_cube(
    SDL_Renderer *renderer,
    const Camera &camera,
    int world_x,
    int world_y,
    int z,
    SDL_Color base,
    int window_width
);
