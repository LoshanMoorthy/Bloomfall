#include "iso_plane.h"

void render_iso_plane(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const Camera &camera,
    int world[][10],
    int map_width,
    int map_height,
    int window_width
) {
    constexpr int tile_width = 64;
    constexpr int tile_height = 32;

    const int origin_x = window_width / 2;
    const int origin_y = 100;

    for (int y{}; y < map_height; y++) {
        for (int x{}; x < map_width; x++) {

            int tile_id = world[x][y];

            if (tile_id == 0)
                continue;

            int screen_x =
                origin_x +
                (x - y) * (tile_width / 2) - static_cast<int>(camera.x);

            int screen_y =
                origin_y +
                (x + y) * (tile_height / 2) - static_cast<int>(camera.y);

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
