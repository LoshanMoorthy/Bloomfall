#pragma once

#include <SDL2/SDL.h>
#include <vector>

struct Player {
    float x = 100.0f;
    float y = 100.0f;
    int size = 50;
    float speed = 300.0f;
    SDL_Rect rect{};
};

void update_player_rect(Player &player);

void move_player(
    Player &player,
    const uint8_t *keys,
    float delta_time,
    const std::vector<SDL_Rect> &walls
);

void clamp_player_to_window(
    Player &player,
    int window_width,
    int window_height
);

void render_player(SDL_Renderer *renderer, const Player &player);
