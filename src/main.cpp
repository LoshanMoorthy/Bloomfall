#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int PLAYER_SIZE = 50;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_init failed: " << SDL_GetError() << "\n";
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

    float player_x = 100.0f;
    float player_y = 100.0f;
    SDL_Rect player{
        static_cast<int>(player_x),
        static_cast<int>(player_y),
        PLAYER_SIZE,
        PLAYER_SIZE
    };
    float speed = 300.0f;
    Uint32 last_tick = SDL_GetTicks();

    bool running = true;
    SDL_Event event;

    while (running) {
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

        if (keys[SDL_SCANCODE_W])
            player_y -= delta_time * speed;
        if (keys[SDL_SCANCODE_S])
            player_y += delta_time * speed;
        if (keys[SDL_SCANCODE_A])
            player_x -= delta_time * speed;
        if (keys[SDL_SCANCODE_D])
            player_x += delta_time * speed;

        player.x = static_cast<int>(player_x);
        player.y = static_cast<int>(player_y);

        SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 80, 180, 120, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
