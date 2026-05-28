#pragma once

#include <SDL2/SDL.h>
#include <string>

struct Texture {
    SDL_Texture *handle = nullptr;
    int width = 0;
    int height = 0;
};

bool load_texture(Texture &texture, SDL_Renderer *renderer, const std::string &path);

void destroy_texture(Texture &texture);
