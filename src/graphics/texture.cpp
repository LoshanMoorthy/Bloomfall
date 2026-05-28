#include "texture.h"

#include <SDL2/SDL_image.h>
#include <iostream>

bool load_texture(
    Texture &texture,
    SDL_Renderer *renderer,
    const std::string &path
) {
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (surface == nullptr) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << "\n";
        return false;
    }

    texture.width = surface->w;
    texture.height = surface->h;

    texture.handle = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture.handle == nullptr) {
        std::cout << "SDL_CreateTextureFromSurface failed " << SDL_GetError() << "\n";
    }

    return true;
}

void destroy_texture(Texture &texture) {
    if (texture.handle != nullptr) {
        SDL_DestroyTexture(texture.handle);
        texture.handle = nullptr;
    }
}
