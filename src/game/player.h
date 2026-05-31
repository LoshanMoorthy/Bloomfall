#pragma once

#include <SDL2/SDL.h>

#include "camera.h"

struct Player {
    float x = 100.0f;
    float y = 100.0f;
    int size = 50;
    float speed = 3.0f;
    SDL_Rect rect{};
};

void update_player_rect(Player &player);

void move_player(
    Player &player,
    const uint8_t *keys,
    float delta_time
);

void clamp_player_to_window(
    Player &player,
    int window_width,
    int window_height
);

void render_player(
    SDL_Renderer *renderer,
    const Camera &camera,
    float world_x,
    float world_y,
    float z,
    SDL_Color,
    int window_width
);
