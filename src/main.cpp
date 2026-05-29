#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include "game/cube.h"
#include "game/player.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

constexpr int map_width = 10;
constexpr int map_height = 10;

int world[map_height][map_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int height_map[map_height][map_width] = {
    {1, 1, 1, 4, 1, 1, 1, 1, 1, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 8, 1, 1, 1, 1},
    {1, 1, 1, 1, 6, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 3, 1, 1, 1, 1, 1},
    {1, 1, 1, 2, 5, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 3, 3, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 3, 1}
};

struct DrawCube {
    int x, y, z;
    SDL_Color color;
};

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "IMG_Init failed: " << IMG_GetError() << "\n";
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Bloomfall",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_Texture *deep_rock_texture = nullptr;

    SDL_Surface *surface =
        IMG_Load("D:/c_projects/Bloomfall/assets/tile/deep_rock.png");

    if (surface == nullptr) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << "\n";
        return 1;
    }

    deep_rock_texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (deep_rock_texture == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface failed: "
                  << SDL_GetError() << "\n";
        return 1;
    }

    Player player;
    player.rect = {
        static_cast<int>(player.x),
        static_cast<int>(player.y),
        player.size,
        player.size
    };
    Uint32 last_tick = SDL_GetTicks();

    std::vector<SDL_Rect> walls{};
    Camera camera;

    bool running = true;
    SDL_Event event;

    while (running) {
        Uint32 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        Uint32 current_tick = SDL_GetTicks();
        Uint32 delta_ms = current_tick - last_tick;
        float delta_time = delta_ms / 1000.0f;

        last_tick = current_tick;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
        SDL_RenderClear(renderer);

        if (keys[SDL_SCANCODE_UP])
            camera.y -= 300.0f * delta_time;

        if (keys[SDL_SCANCODE_DOWN])
            camera.y += 300.0f * delta_time;

        if (keys[SDL_SCANCODE_LEFT])
            camera.x -= 300.0f * delta_time;

        if (keys[SDL_SCANCODE_RIGHT])
            camera.x += 300.0f * delta_time;

        std::vector<DrawCube> cubes;
        for (int y{}; y < map_height; y++) {
            for (int x{}; x < map_width; x++) {
                int h = height_map[y][x];
                for (int z{}; z < h; z++) {
                    cubes.push_back({x, y, z, SDL_Color{120, 120, 120, 155}});
                }
            }
        }

        std::sort(cubes.begin(), cubes.end(), [](const DrawCube &a, const DrawCube &b) {
            if (a.x + a.y != b.x + b.y)
                return (a.x + a.y) < (b.x + b.y);
            return a.z < b.z;
        });

        for (const DrawCube &c : cubes) {
            render_cube(renderer, camera, c.x, c.y, c.z, c.color, WINDOW_WIDTH);
        };

        move_player(player, keys, delta_time, walls);
        clamp_player_to_window(player, WINDOW_WIDTH, WINDOW_HEIGHT);
        update_player_rect(player);

        render_player(renderer, player);

        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - frame_start;

        if (frame_time < FRAME_DELAY_MS) {
            SDL_Delay(FRAME_DELAY_MS - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
