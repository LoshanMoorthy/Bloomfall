#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int TARGET_FPS = 144;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

struct Player {
    float x = 100.0f;
    float y = 100.0f;
    int size = 50;
    float speed = 300.0f;
    SDL_Rect rect{};
};

void update_player_rect(Player &player) {
    player.rect.x = static_cast<int>(player.x);
    player.rect.y = static_cast<int>(player.y);
    player.rect.w = player.size;
    player.rect.h = player.size;
}

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

    Player player;
    player.rect = {
        static_cast<int>(player.x),
        static_cast<int>(player.y),
        player.size,
        player.size
    };
    Uint32 last_tick = SDL_GetTicks();

    std::vector<SDL_Rect> walls{
        {500, 250, 100, 250},
        {200, 150, 300, 50},
        {800, 400, 50, 200}
    };
    SDL_Rect wall{500, 250, 100, 250};

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
        if (player.x < 0.0f)
            player.x = 0.0f;

        if (player.x > WINDOW_WIDTH - player.size)
            player.x = static_cast<float>(WINDOW_WIDTH - player.size);

        if (player.y < 0.0f)
            player.y = 0.0f;

        if (player.y > WINDOW_HEIGHT - player.size)
            player.y = static_cast<float>(WINDOW_HEIGHT - player.size);

        update_player_rect(player);

        SDL_SetRenderDrawColor(renderer, 80, 180, 120, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        SDL_SetRenderDrawColor(renderer, 180, 80, 80, 255);
        for (const SDL_Rect &wall : walls) {
            SDL_RenderFillRect(renderer, &wall);
        }

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
