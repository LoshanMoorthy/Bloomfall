#include "player.h"

void update_player_rect(Player &player) {
    player.rect.x = static_cast<int>(player.x);
    player.rect.y = static_cast<int>(player.y);
    player.rect.w = player.size;
    player.rect.h = player.size;
}

void move_player(
    Player &player,
    const uint8_t *keys,
    float delta_time,
    const std::vector<SDL_Rect> &walls
) {
    float old_x = player.x;

    if (keys[SDL_SCANCODE_A])
        player.x -= player.speed * delta_time;
    if (keys[SDL_SCANCODE_D])
        player.x += player.speed * delta_time;

    update_player_rect(player);

    // AABB collision check
    for (const SDL_Rect &wall : walls) {
        if (SDL_HasIntersection(&player.rect, &wall)) {
            player.x = old_x;
            update_player_rect(player);
        }
    }

    float old_y = player.y;

    if (keys[SDL_SCANCODE_W])
        player.y -= player.speed * delta_time;
    if (keys[SDL_SCANCODE_S])
        player.y += player.speed * delta_time;

    update_player_rect(player);

    // AABB collision check
    for (const SDL_Rect &wall : walls) {
        if (SDL_HasIntersection(&player.rect, &wall)) {
            player.y = old_y;
            update_player_rect(player);
        }
    }
}

void clamp_player_to_window(
    Player &player,
    int window_width,
    int window_height
) {
    if (player.x < 0.0f)
        player.x = 0.0f;

    if (player.x > window_width - player.size)
        player.x = static_cast<float>(window_width - player.size);

    if (player.y < 0.0f)
        player.y = 0.0f;

    if (player.y > window_height - player.size)
        player.y = static_cast<float>(window_height - player.size);
}

void render_player(SDL_Renderer *renderer, const Player &player) {
    SDL_SetRenderDrawColor(renderer, 80, 180, 120, 255);
    SDL_RenderFillRect(renderer, &player.rect);
}
