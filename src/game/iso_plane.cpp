#include "iso_plane.h"

void render_iso_plane(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    int window_width
) {
    constexpr int tile_width = 64;
    constexpr int tile_height = 32;

    constexpr int map_width = 10;
    constexpr int map_height = 10;

    const int origin_x = window_width / 2;
    const int origin_y = 80;

    for (int y{}; y < map_height; y++) {
        for (int x{}; x < map_width; x++) {
            int screen_x = origin_x + (x - y) * (tile_width / 2);
            int screen_y = origin_y + (x + y) * (tile_height / 2);

            SDL_Rect dst{
                screen_x,
                screen_y,
                tile_width,
                tile_height
            };

            SDL_RenderCopy(renderer, texture, nullptr, &dst);
        }
    }
}
